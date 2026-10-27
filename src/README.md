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

`../build-quantum.bat` builds every tool from this directory with no C++
compiler. `../seed/qrun.exe` is qrun compiled to x86-64 by `native/`:
- it compiles `tools/selfhost/qrun_all.sa` (all of qrun: compiler and VM) to a
  new native qrun;
- that qrun compiles itself again, then `quantum_all.sa`, `stub_all.sa` and
  `qpm_all.sa` (quantum.exe, quantum_stub.exe, qpm.exe).

The drivers include every source as its own translation unit
(`#pragma quantum_tu`), the files CMake builds for each target. `--verify`
checks the fixed point; `tools/native_bootstrap.sh` checks that qrun built by
g++, by itself and by that build are all byte-identical.

## The native backend

`native/` compiles C++ straight to an x86-64 Windows executable: no
assembler, linker or C++ compiler (`quantum --native prog.cpp prog.exe`).
`tools/native_test.sh` checks it against `../tests/native/`. It handles:
- integers, floating point, pointers, arrays
- classes with constructors, destructors, inheritance and virtual functions
- references, operator overloading, function and class templates
- lambdas, exceptions, threads and atomics
- the standard library in `../lib/native/` (strings, containers, streams,
  filesystem, regex, random, chrono, zlib, the Windows API), written in C++
  and carried inside the compiler (`tools/embed.sa` writes `generated/`)

It compiles the whole of Quantum: qrun, quantum, quantum_stub and qpm.
