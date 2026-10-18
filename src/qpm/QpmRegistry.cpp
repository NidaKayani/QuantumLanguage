#include "QpmRegistry.h"
#include "QpmHttp.h"
#include "QpmJson.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

namespace qpm
{
    const char *const kDefaultRegistry = "https://qpm.quantumlogicslimited.com/api/registry/";

    namespace
    {
        std::string g_registryOverride;

        std::string envOr(const char *name, const std::string &def)
        {
            const char *v = std::getenv(name);
            return (v && *v) ? std::string(v) : def;
        }

        fs::path rcPath()
        {
            std::string home = envOr("USERPROFILE", envOr("HOME", "."));
            return fs::path(home) / ".qpmrc";
        }

        JsonValue loadRc()
        {
            std::ifstream f(rcPath(), std::ios::binary);
            if (!f)
                return JsonValue::makeObject();
            std::ostringstream ss;
            ss << f.rdbuf();
            try
            {
                JsonValue rc = JsonValue::parse(ss.str());
                if (rc.isObject())
                    return rc;
            }
            catch (...)
            {
            }
            return JsonValue::makeObject();
        }

        bool saveRc(const JsonValue &rc)
        {
            std::ofstream f(rcPath(), std::ios::binary | std::ios::trunc);
            if (!f)
                return false;
            f << rc.stringify(2) << "\n";
            return static_cast<bool>(f);
        }

        void setSavedToken(const std::string &registry, const std::string &token)
        {
            JsonValue rc = loadRc();
            JsonValue tokens = rc.get("tokens");
            if (!tokens.isObject())
                tokens = JsonValue::makeObject();

            if (token.empty())
            {
                JsonValue kept = JsonValue::makeObject();
                for (const auto &m : tokens.object())
                    if (m.first != registry)
                        kept.set(m.first, m.second);
                tokens = kept;
            }
            else
            {
                tokens.set(registry, token);
            }
            rc.set("tokens", tokens);
            if (!saveRc(rc))
                std::cerr << "[qpm] warning: could not write " << rcPath().string() << "\n";
        }

        // The auth API is the registry's sibling: ".../api/registry/" -> ".../api/auth/".
        std::string authBaseUrl()
        {
            std::string base = registryBaseUrl();
            base.pop_back(); // trailing '/'
            size_t slash = base.find_last_of('/');
            return base.substr(0, slash + 1) + "auth/";
        }

        std::string prompt(const std::string &label, bool hideInput)
        {
            std::cout << label << std::flush;
#ifdef _WIN32
            HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
            DWORD mode = 0;
            bool restore = hideInput && GetConsoleMode(in, &mode) &&
                           SetConsoleMode(in, mode & ~ENABLE_ECHO_INPUT);
#else
            bool restore = false;
#endif
            std::string line;
            std::getline(std::cin, line);
#ifdef _WIN32
            if (restore)
            {
                SetConsoleMode(in, mode);
                std::cout << "\n";
            }
#endif
            if (!line.empty() && line.back() == '\r')
                line.pop_back();
            return line;
        }

        // The site that lists a registry's packages: the registry's own origin,
        // except where the API is served from another host than the website.
        std::string websiteBaseUrl(const std::string &registry)
        {
            if (!isQpmRegistry(registry))
                return registry.find("//registry.npmjs.org/") != std::string::npos ? "https://www.npmjs.com/" : "";
            std::string origin = registry.substr(0, registry.size() - std::strlen("api/registry/"));
            if (origin == "https://qpm-website-backend.vercel.app/")
                return "https://qpm.quantumlogicslimited.com/";
            // The local backend's UI is the Vite dev server, which proxies /api to it.
            if (origin == "http://localhost:8000/" || origin == "http://127.0.0.1:8000/")
                return "http://localhost:3000/";
            return origin;
        }
    } // namespace

    void setRegistryOverride(const std::string &url)
    {
        g_registryOverride = url;
    }

    std::string registryBaseUrl()
    {
        std::string base = g_registryOverride.empty() ? envOr("QPM_REGISTRY", kDefaultRegistry) : g_registryOverride;
        if (base.back() != '/')
            base += '/';
        return base;
    }

    bool isQpmRegistry(const std::string &registry)
    {
        const std::string suffix = "/api/registry/";
        return registry.size() > suffix.size() &&
               registry.compare(registry.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    std::string packagePageUrl(const std::string &registry, const std::string &name)
    {
        std::string site = websiteBaseUrl(registry);
        return site.empty() ? "" : site + "package/" + name;
    }

    std::string upstreamRegistryUrl()
    {
        std::string base = envOr("QPM_UPSTREAM", "https://registry.npmjs.org/");
        if (base == "none")
            return "";
        if (base.back() != '/')
            base += '/';
        return base;
    }

    std::string authToken()
    {
        std::string fromEnv = envOr("QPM_TOKEN", "");
        if (!fromEnv.empty())
            return fromEnv;
        return loadRc().get("tokens").get(registryBaseUrl()).asString();
    }

    int runLogin()
    {
        std::string registry = registryBaseUrl();
        std::cout << "[qpm] log in to " << registry << "\n";
        std::string user = prompt("Username or email: ", false);
        std::string password = prompt("Password: ", true);
        if (user.empty() || password.empty())
        {
            std::cerr << "[qpm] username and password are required\n";
            return 1;
        }

        JsonValue body = JsonValue::makeObject();
        body.set("emailOrUsername", user);
        body.set("password", password);
        HttpResponse resp = httpRequest("POST", authBaseUrl() + "login", body.stringify(),
                                        {"Content-Type: application/json", "Accept: application/json"});
        if (!resp.ok())
        {
            std::cerr << "[qpm] login failed (" << describeFailure(resp) << ")\n";
            std::string site = websiteBaseUrl(registry);
            if (resp.status == 401 && !site.empty())
                std::cerr << "[qpm] no account yet? sign up at " << site << "\n";
            return 1;
        }

        JsonValue doc;
        try
        {
            doc = JsonValue::parse(resp.body);
        }
        catch (...)
        {
        }
        std::string token = doc.get("token").asString();
        if (token.empty())
        {
            std::cerr << "[qpm] login failed (registry returned no token)\n";
            return 1;
        }

        setSavedToken(registry, token);
        std::cout << "[qpm] logged in as " << doc.get("user").get("username").asString(user) << "\n";
        return 0;
    }

    int runLogout()
    {
        std::string registry = registryBaseUrl();
        if (loadRc().get("tokens").get(registry).asString().empty())
        {
            std::cout << "[qpm] not logged in to " << registry << "\n";
            return 0;
        }
        setSavedToken(registry, "");
        std::cout << "[qpm] logged out of " << registry << "\n";
        return 0;
    }

    bool lookupUser(const std::string &token, std::string &username, std::string &error)
    {
        HttpResponse resp = httpRequest("GET", authBaseUrl() + "me", "",
                                        {"Accept: application/json", "Authorization: Bearer " + token});
        if (!resp.ok())
        {
            error = describeFailure(resp);
            return false;
        }
        try
        {
            username = JsonValue::parse(resp.body).get("user").get("username").asString();
        }
        catch (...)
        {
        }
        if (username.empty())
            username = "(unknown user)";
        return true;
    }

    int runWhoami()
    {
        std::string token = authToken();
        if (token.empty())
        {
            std::cerr << "[qpm] not logged in (run `qpm login`)\n";
            return 1;
        }

        std::string username, error;
        if (!lookupUser(token, username, error))
        {
            std::cerr << "[qpm] " << error << " — run `qpm login` again\n";
            return 1;
        }
        std::cout << username << "\n";
        return 0;
    }

} // namespace qpm
