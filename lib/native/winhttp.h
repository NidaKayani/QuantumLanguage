// <winhttp.h> for Quantum's native backend: the WinHTTP client API
// (winhttp.dll) — sessions, connections, requests, URL cracking.
#pragma once
#include <windows.h>

typedef void *HINTERNET;
typedef WORD INTERNET_PORT;
typedef int INTERNET_SCHEME;

#define INTERNET_SCHEME_HTTP 1
#define INTERNET_SCHEME_HTTPS 2
#define INTERNET_DEFAULT_PORT 0
#define INTERNET_DEFAULT_HTTP_PORT 80
#define INTERNET_DEFAULT_HTTPS_PORT 443

struct URL_COMPONENTS
{
    DWORD dwStructSize;
    LPWSTR lpszScheme;
    DWORD dwSchemeLength;
    INTERNET_SCHEME nScheme;
    LPWSTR lpszHostName;
    DWORD dwHostNameLength;
    INTERNET_PORT nPort;
    LPWSTR lpszUserName;
    DWORD dwUserNameLength;
    LPWSTR lpszPassword;
    DWORD dwPasswordLength;
    LPWSTR lpszUrlPath;
    DWORD dwUrlPathLength;
    LPWSTR lpszExtraInfo;
    DWORD dwExtraInfoLength;
};
typedef URL_COMPONENTS URL_COMPONENTSW;

#define WINHTTP_ACCESS_TYPE_DEFAULT_PROXY 0
#define WINHTTP_ACCESS_TYPE_NO_PROXY 1
#define WINHTTP_ACCESS_TYPE_NAMED_PROXY 3
#define WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY 4
#define WINHTTP_NO_PROXY_NAME nullptr
#define WINHTTP_NO_PROXY_BYPASS nullptr
#define WINHTTP_NO_REFERER nullptr
#define WINHTTP_DEFAULT_ACCEPT_TYPES nullptr
#define WINHTTP_NO_ADDITIONAL_HEADERS nullptr
#define WINHTTP_NO_REQUEST_DATA nullptr
#define WINHTTP_HEADER_NAME_BY_INDEX nullptr
#define WINHTTP_NO_OUTPUT_BUFFER nullptr
#define WINHTTP_NO_HEADER_INDEX nullptr
#define WINHTTP_FLAG_SECURE 0x00800000
#define WINHTTP_FLAG_REFRESH 0x00000100
#define WINHTTP_ADDREQ_FLAG_ADD_IF_NEW 0x10000000
#define WINHTTP_ADDREQ_FLAG_ADD 0x20000000
#define WINHTTP_ADDREQ_FLAG_REPLACE 0x80000000
#define WINHTTP_QUERY_CONTENT_TYPE 1
#define WINHTTP_QUERY_CONTENT_LENGTH 5
#define WINHTTP_QUERY_STATUS_CODE 19
#define WINHTTP_QUERY_STATUS_TEXT 20
#define WINHTTP_QUERY_RAW_HEADERS_CRLF 22
#define WINHTTP_QUERY_LOCATION 33
#define WINHTTP_QUERY_FLAG_NUMBER 0x20000000
#define WINHTTP_OPTION_REDIRECT_POLICY 88
#define ICU_ESCAPE 0x80000000
#define ICU_DECODE 0x10000000

namespace __dll_winhttp
{
HINTERNET WinHttpOpen(LPCWSTR agent, DWORD accessType, LPCWSTR proxy, LPCWSTR proxyBypass, DWORD flags);
BOOL WinHttpSetTimeouts(HINTERNET handle, int resolveTimeout, int connectTimeout, int sendTimeout,
                        int receiveTimeout);
HINTERNET WinHttpConnect(HINTERNET session, LPCWSTR server, INTERNET_PORT port, DWORD reserved);
HINTERNET WinHttpOpenRequest(HINTERNET connect, LPCWSTR verb, LPCWSTR object, LPCWSTR version, LPCWSTR referrer,
                             LPCWSTR *acceptTypes, DWORD flags);
BOOL WinHttpAddRequestHeaders(HINTERNET request, LPCWSTR headers, DWORD length, DWORD modifiers);
BOOL WinHttpSendRequest(HINTERNET request, LPCWSTR headers, DWORD headersLength, LPVOID optional,
                        DWORD optionalLength, DWORD totalLength, DWORD_PTR context);
BOOL WinHttpReceiveResponse(HINTERNET request, LPVOID reserved);
BOOL WinHttpQueryHeaders(HINTERNET request, DWORD infoLevel, LPCWSTR name, LPVOID buffer, LPDWORD bufferLength,
                         LPDWORD index);
BOOL WinHttpQueryDataAvailable(HINTERNET request, LPDWORD available);
BOOL WinHttpReadData(HINTERNET request, LPVOID buffer, DWORD toRead, LPDWORD read);
BOOL WinHttpWriteData(HINTERNET request, LPCVOID buffer, DWORD toWrite, LPDWORD written);
BOOL WinHttpSetOption(HINTERNET handle, DWORD option, LPVOID buffer, DWORD length);
BOOL WinHttpCloseHandle(HINTERNET handle);
BOOL WinHttpCrackUrl(LPCWSTR url, DWORD length, DWORD flags, URL_COMPONENTS *components);
} // namespace __dll_winhttp
using namespace __dll_winhttp;
