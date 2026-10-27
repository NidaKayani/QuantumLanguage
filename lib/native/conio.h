// <conio.h> for Quantum's native backend: console key input from the C
// runtime (ucrtbase, as a g++ build calls it).
#pragma once

typedef unsigned short wint_t;

namespace __dll_ucrtbase
{
int _getch();
wint_t _getwch();
int _getche();
int _kbhit();
int _putch(int c);
wint_t _putwch(wchar_t c);
} // namespace __dll_ucrtbase
using namespace __dll_ucrtbase;
