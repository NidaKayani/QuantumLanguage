# src/

The source of Quantum, written in Quantum: lexer, parser, bytecode compiler,
VM, the other language front-ends (C/C++, Java, Python, JS, Ruby), CLI and
qpm.

Most of it is Quantum's C++ syntax. Such a file starts with the line
`// quantum: cpp`; `X.h.sa` declares and `X.sa` defines. `lexer.sa`, `ast.sa`
and `parser*.sa` are a separate port of the lexer and parser in Quantum's own
syntax.

## Self-hosting

`tools/selfhost/quantumc.sa` is the whole compiler as one program. Quantum
compiles it, and it then compiles any Quantum program on the Quantum VM:

    qrun --emit-qbc src/tools/selfhost/quantumc.sa build/quantumc.qbc
    qrun --exec build/quantumc.qbc prog.sa prog.qbc

Checks:

| Check                              | What it shows                                            |
|------------------------------------|----------------------------------------------------------|
| `tools/compare_selfhost.sh full`   | the self-hosted compiler writes the same `.qbc` as `qrun` on every example |
| `tools/compare_selfhost.sh tokens` / `ast` / `qbc` | the same for the lexer, the parser, and the bytecode compiler alone |
| `tools/bootstrap.sh`               | it compiles itself to a fixed point: q1 == q2 == q3      |
| `tools/cpp_coverage.sh`            | which files compile on their own (strict, no fallback)   |

## What still needs a C++ compiler

The `.exe` binaries (`quantum`, `qrun`, `quantum_stub`, `qpm`). CMake compiles
these same `.sa` files with g++ (`-x c++`, see `../CMakeLists.txt`). Removing
that step needs Quantum's native x86-64 PE backend, so that Quantum can compile
the VM to machine code itself.
