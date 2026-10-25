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

## Building without a C++ compiler

`../build-quantum.bat` builds `quantum.exe` from this directory with no C++
compiler, using `tools/selfhost/quantum.sa`, the compiler's command-line
driver:
- `../seed/quantum.exe` (the VM plus that driver as bytecode) compiles it;
- the result compiles itself again.

`--verify` checks the fixed point.

The VM runtime (`vm/`, `Value.sa`) runs as native code inside the seed, so
an edit to it needs the VM recompiled. Until `native/` can compile the VM, that
step uses CMake and g++ (`../CMakeLists.txt`).

## The native backend

`native/` compiles C++ straight to an x86-64 Windows executable: no
assembler, linker or C++ compiler (`quantum --native prog.cpp prog.exe`).
`tools/native_test.sh` checks it against `../tests/native/`. It handles:
- integers, floating point, pointers, arrays
- classes with constructors, destructors, inheritance and virtual functions
- references, operator overloading, function and class templates
- lambdas, exceptions

The standard library comes next, then the VM itself.
