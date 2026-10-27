// <shellapi.h> for Quantum's native backend: the shell32 functions used.
#pragma once
#include <windows.h>

namespace __dll_shell32
{
HINSTANCE ShellExecuteA(HWND window, LPCSTR operation, LPCSTR file, LPCSTR parameters, LPCSTR directory, INT show);
HINSTANCE ShellExecuteW(HWND window, LPCWSTR operation, LPCWSTR file, LPCWSTR parameters, LPCWSTR directory,
                        INT show);
} // namespace __dll_shell32
