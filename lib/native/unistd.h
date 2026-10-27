// <unistd.h> for Quantum's native backend: the POSIX names the C runtime
// (ucrtbase) has, under their underscored names.
#pragma once

namespace __dll_ucrtbase
{
int _isatty(int fd);
int _access(const char *path, int mode);
int _getpid();
} // namespace __dll_ucrtbase
inline int isatty(int fd) { return __dll_ucrtbase::_isatty(fd); }
inline int access(const char *path, int mode) { return __dll_ucrtbase::_access(path, mode); }
inline int getpid() { return __dll_ucrtbase::_getpid(); }
#define F_OK 0
#define W_OK 2
#define R_OK 4
