// <windows.h> for Quantum's native backend: the Windows API functions the
// library uses. A function declared in namespace __dll_<name> is imported
// from <name>.dll (the compiler writes the import table).
#pragma once

typedef void *HANDLE;
typedef void *HMODULE;
typedef void *HLOCAL;
typedef void *LPVOID;
typedef const void *LPCVOID;
typedef unsigned DWORD;
typedef unsigned short WORD;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned UINT;
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;
typedef unsigned long long SIZE_T;
typedef wchar_t WCHAR;
typedef wchar_t *LPWSTR;
typedef const wchar_t *LPCWSTR;
typedef char *LPSTR;
typedef const char *LPCSTR;
typedef DWORD *LPDWORD;

struct FILETIME
{
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
};

struct WIN32_FIND_DATAW
{
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    wchar_t cFileName[260];
    wchar_t cAlternateFileName[14];
};

struct SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
};
namespace __dll_kernel32
{
BOOL FileTimeToLocalFileTime(const FILETIME *utc, FILETIME *local);
BOOL FileTimeToSystemTime(const FILETIME *time, SYSTEMTIME *out);
BOOL SystemTimeToFileTime(const SYSTEMTIME *in, FILETIME *time);
BOOL LocalFileTimeToFileTime(const FILETIME *local, FILETIME *utc);
HANDLE CreateThread(LPVOID security, SIZE_T stack, LPVOID start, LPVOID param, DWORD flags, LPDWORD id);
DWORD WaitForSingleObject(HANDLE h, DWORD ms);
unsigned long long GetTickCount64();
HANDLE GetStdHandle(DWORD which);
BOOL WriteFile(HANDLE file, LPCVOID buffer, DWORD count, LPDWORD written, LPVOID overlapped);
BOOL ReadFile(HANDLE file, LPVOID buffer, DWORD count, LPDWORD read, LPVOID overlapped);
HANDLE CreateFileW(LPCWSTR name, DWORD access, DWORD share, LPVOID security, DWORD disposition, DWORD flags,
                   HANDLE templateFile);
BOOL CloseHandle(HANDLE h);
BOOL GetFileSizeEx(HANDLE file, long long *size);
BOOL SetFilePointerEx(HANDLE file, long long distance, long long *newPosition, DWORD method);
DWORD GetFileAttributesW(LPCWSTR name);
BOOL CreateDirectoryW(LPCWSTR name, LPVOID security);
BOOL RemoveDirectoryW(LPCWSTR name);
BOOL DeleteFileW(LPCWSTR name);
BOOL CopyFileW(LPCWSTR from, LPCWSTR to, BOOL failIfExists);
BOOL MoveFileExW(LPCWSTR from, LPCWSTR to, DWORD flags);
HANDLE FindFirstFileW(LPCWSTR pattern, WIN32_FIND_DATAW *data);
BOOL FindNextFileW(HANDLE find, WIN32_FIND_DATAW *data);
BOOL FindClose(HANDLE find);
DWORD GetCurrentDirectoryW(DWORD size, LPWSTR buffer);
BOOL SetCurrentDirectoryW(LPCWSTR name);
DWORD GetFullPathNameW(LPCWSTR name, DWORD size, LPWSTR buffer, LPWSTR *filePart);
DWORD GetModuleFileNameW(HMODULE module, LPWSTR buffer, DWORD size);
DWORD GetEnvironmentVariableW(LPCWSTR name, LPWSTR buffer, DWORD size);
LPWSTR GetCommandLineW();
HLOCAL LocalFree(HLOCAL mem);
DWORD GetLastError();
void ExitProcess(UINT code);
int MultiByteToWideChar(UINT codePage, DWORD flags, LPCSTR text, int count, LPWSTR wide, int wideCount);
int WideCharToMultiByte(UINT codePage, DWORD flags, LPCWSTR wide, int count, LPSTR text, int textCount,
                        LPCSTR defaultChar, BOOL *usedDefault);
BOOL QueryPerformanceCounter(long long *count);
BOOL QueryPerformanceFrequency(long long *frequency);
void GetSystemTimeAsFileTime(FILETIME *time);
void Sleep(DWORD milliseconds);
BOOL SetConsoleOutputCP(UINT codePage);
BOOL SetConsoleCP(UINT codePage);
DWORD GetTickCount();
} // namespace __dll_kernel32

namespace __dll_shell32
{
LPWSTR *CommandLineToArgvW(LPCWSTR commandLine, int *count);
} // namespace __dll_shell32

using namespace __dll_kernel32;
using namespace __dll_shell32;

const DWORD STD_INPUT_HANDLE = (DWORD)-10;
const DWORD STD_OUTPUT_HANDLE = (DWORD)-11;
const DWORD STD_ERROR_HANDLE = (DWORD)-12;
const DWORD GENERIC_READ = 0x80000000;
const DWORD GENERIC_WRITE = 0x40000000;
const DWORD FILE_SHARE_READ = 1;
const DWORD FILE_SHARE_WRITE = 2;
const DWORD CREATE_ALWAYS = 2;
const DWORD OPEN_EXISTING = 3;
const DWORD OPEN_ALWAYS = 4;
const DWORD FILE_ATTRIBUTE_NORMAL = 0x80;
const DWORD FILE_ATTRIBUTE_DIRECTORY = 0x10;
const DWORD INVALID_FILE_ATTRIBUTES = 0xFFFFFFFF;
const UINT CP_UTF8 = 65001;
#define TRUE 1
#define FALSE 0
#define MAX_PATH 260
#define INFINITE 0xFFFFFFFF
#define INVALID_HANDLE_VALUE ((HANDLE)(long long)-1)
