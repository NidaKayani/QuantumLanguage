// qpm — the Quantum Package Manager.
//
// A standalone, from-scratch npm-compatible installer, publisher + script
// runner: it talks to the (npm-compatible) QPM registry directly over HTTPS
// (via WinHTTP) and packs/unpacks tarballs itself (via zlib + a small tar
// reader/writer), so it needs neither npm nor Node.js installed to install or
// publish packages. Running the
// dependencies' own scripts (`qpm run` / `qpm start`) still shells out to
// whatever the script names — same as npm, qpm doesn't include a JS engine.

#include "QpmBuild.h"
#include "QpmPublish.h"
#include "QpmRegistry.h"
#include "QpmResolver.h"
#include "QpmScripts.h"

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
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace
{
    void printHelp()
    {
        std::cout <<
            "qpm - Quantum Package Manager\n"
            "\n"
            "Usage:\n"
            "  qpm init                    creates a package.json (asks for each field)\n"
            "  qpm init -y                 creates a package.json with defaults, no questions\n"
            "  qpm install                 installs all dependencies from the manifest\n"
            "  qpm install <pkg> [...]     adds and installs one or more packages\n"
            "  qpm install --no-dev        skip devDependencies\n"
            "  qpm i / qpm add             aliases for `qpm install`\n"
            "  qpm run <script>            runs a manifest \"scripts\" entry\n"
            "  qpm start                   shorthand for `qpm run start`\n"
            "  qpm build                   runs the \"build\" script, or builds a C/C++ project\n"
            "                              (CMakeLists.txt / Makefile / plain sources) natively\n"
            "  qpm build [name] --run      builds, then runs the program (args after --)\n"
            "  qpm build --debug           debug build (default --release)\n"
            "  qpm build --clean           removes the build outputs\n"
            "  qpm build --cmake | --make | --direct   picks how a C/C++ project is built\n"
            "  qpm build -j <n> / -v       parallel compiles / print each command\n"
            "  qpm publish                 packs this package and uploads it to the registry\n"
            "  qpm publish --dry-run       shows what would be published, uploads nothing\n"
            "  qpm publish --token <t>     publish with this token instead of the saved login\n"
            "  qpm pack                    writes <name>-<version>.tgz without publishing\n"
            "  qpm login                   logs in to the registry and saves the token\n"
            "  qpm logout                  forgets the saved token\n"
            "  qpm whoami                  shows which account you're logged in as\n"
            "  qpm --help                  show this help\n"
            "  qpm --version               show version\n"
            "\n"
            "Manifest (the first one found in the project folder is used):\n"
            "  package.qon       JSON like package.json, or requirements style\n"
            "  package.json      npm's manifest\n"
            "  requirements.txt  one `name==1.2.3` / `name ^1.2` per line\n"
            "\n"
            "Environment:\n"
            "  QPM_REGISTRY   registry API base (default http://localhost:8000/api/registry/)\n"
            "  QPM_UPSTREAM   where install looks up packages the registry doesn't have\n"
            "                 (default https://registry.npmjs.org/, \"none\" to turn off)\n"
            "  QPM_TOKEN      auth token, overrides the one saved by `qpm login`\n"
            "\n"
            "qpm downloads packages straight from the registry — no npm or Node.js\n"
            "install required. Running a script (`qpm run`/`qpm start`) still shells\n"
            "out to whatever that script names (often `node ...`), so a script that\n"
            "itself invokes Node.js still needs Node.js present to execute.\n";
    }

    int runPublishCommand(const std::vector<std::string> &args, const std::string &cwd)
    {
        qpm::PublishOptions opts;
        opts.projectDir = cwd;
        for (size_t i = 1; i < args.size(); ++i)
        {
            if (args[i] == "--dry-run")
            {
                opts.dryRun = true;
            }
            else if (args[i] == "--token" && i + 1 < args.size())
            {
                opts.token = args[++i];
            }
            else
            {
                std::cerr << "[qpm] unknown publish option: " << args[i] << "\n"
                          << "[qpm] usage: qpm publish [--dry-run] [--token <token>]\n";
                return 1;
            }
        }
        return qpm::runPublish(opts);
    }

    int runBuildCommand(const std::vector<std::string> &args, const std::string &cwd)
    {
        qpm::BuildOptions opts;
        opts.projectDir = cwd;
        const char *usage = "[qpm] usage: qpm build [name] [--run] [--debug|--release] [--clean]"
                            " [--cmake|--make|--direct] [-j <n>] [-v] [-- <program args>]\n";
        for (size_t i = 1; i < args.size(); ++i)
        {
            const std::string &a = args[i];
            if (a == "--")
            {
                opts.runArgs.assign(args.begin() + static_cast<std::ptrdiff_t>(i) + 1, args.end());
                opts.run = true;
                break;
            }
            if (a == "--run" || a == "-r")
                opts.run = true;
            else if (a == "--debug")
                opts.release = false;
            else if (a == "--release")
                opts.release = true;
            else if (a == "--clean")
                opts.clean = true;
            else if (a == "--verbose" || a == "-v")
                opts.verbose = true;
            else if (a == "--cmake")
                opts.backend = qpm::BuildBackend::CMake;
            else if (a == "--make")
                opts.backend = qpm::BuildBackend::Make;
            else if (a == "--direct")
                opts.backend = qpm::BuildBackend::Direct;
            else if ((a == "-j" || a == "--jobs") && i + 1 < args.size())
                opts.jobs = std::atoi(args[++i].c_str());
            else if (a.size() > 2 && a.compare(0, 2, "-j") == 0)
                opts.jobs = std::atoi(a.c_str() + 2);
            else if (!a.empty() && a[0] != '-' && opts.target.empty())
                opts.target = a;
            else
            {
                std::cerr << "[qpm] unknown build option: " << a << "\n" << usage;
                return 1;
            }
        }
        return qpm::runBuild(opts);
    }
}

int main(int argc, char *argv[])
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::vector<std::string> args(argv + 1, argv + argc);
    std::string cwd = fs::current_path().string();

    if (args.empty() || args[0] == "--help" || args[0] == "-h")
    {
        printHelp();
        return 0;
    }
    if (args[0] == "--version" || args[0] == "-v")
    {
        std::cout << "qpm 1.0.0\n";
        return 0;
    }

    const std::string &cmd = args[0];

    if (cmd == "install" || cmd == "i" || cmd == "add")
    {
        qpm::InstallOptions opts;
        opts.projectDir = cwd;
        for (size_t i = 1; i < args.size(); ++i)
        {
            if (args[i] == "--no-dev")
                opts.includeDev = false;
            else
                opts.addPackages.push_back(args[i]);
        }
        return qpm::runInstall(opts);
    }

    if (cmd == "run" || cmd == "run-script")
    {
        if (args.size() < 2)
        {
            std::cerr << "[qpm] usage: qpm run <script>\n";
            return 1;
        }
        return qpm::runScript(cwd, args[1]);
    }

    if (cmd == "start")
        return qpm::runScript(cwd, "start");
    if (cmd == "build")
        return runBuildCommand(args, cwd);
    if (cmd == "init")
    {
        bool yes = false;
        for (size_t i = 1; i < args.size(); ++i)
        {
            if (args[i] == "-y" || args[i] == "--yes")
            {
                yes = true;
            }
            else
            {
                std::cerr << "[qpm] unknown init option: " << args[i] << "\n"
                          << "[qpm] usage: qpm init [-y]\n";
                return 1;
            }
        }
        return qpm::runInit(cwd, yes);
    }
    if (cmd == "publish")
        return runPublishCommand(args, cwd);
    if (cmd == "pack")
        return qpm::runPack(cwd);
    if (cmd == "login")
        return qpm::runLogin();
    if (cmd == "logout")
        return qpm::runLogout();
    if (cmd == "whoami")
        return qpm::runWhoami();

    std::cerr << "[qpm] unknown command: " << cmd << "\n";
    printHelp();
    return 1;
}
