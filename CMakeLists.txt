cmake_minimum_required(VERSION 3.16)
project(Quantum CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(MSVC)
    add_compile_options(/O2 /W3)
else()
    add_compile_options(-O2 -Wall -Wextra -Wno-unused-parameter)
    # Static-link the MinGW/GCC runtime so the .exe works without any DLLs
    add_link_options(-static -static-libgcc -static-libstdc++)
endif()

# The compiler's own source is Quantum, in src/: files whose first line is
# `// quantum: cpp` are Quantum in C++ syntax (X.h.sa declares, X.sa defines).
# Until Quantum builds its own binaries, g++ compiles those same files as C++.
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# ── Collect sources ────────────────────────────────────────────────────────────
file(GLOB_RECURSE ALL_SA CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.sa")
set(CPP_SA)
foreach(f ${ALL_SA})
    if(f MATCHES "/src/tools/" OR f MATCHES "[.]h[.]sa$")
        continue()
    endif()
    file(STRINGS "${f}" first LIMIT_COUNT 1)
    if(first MATCHES "^// quantum: cpp")
        list(APPEND CPP_SA "${f}")
    endif()
endforeach()
set_source_files_properties(${CPP_SA} PROPERTIES LANGUAGE CXX COMPILE_OPTIONS "-xc++")

set(QUANTUM_SOURCES ${CPP_SA})
# qpm is a standalone tool (its own main(), no VM/lexer/parser/compiler needed)
# built from src/qpm/ separately — keep it out of the other 3 targets' sources...
list(FILTER QUANTUM_SOURCES EXCLUDE REGEX ".*/src/qpm/.*[.]sa$")
# ...except its self-contained manifest readers: `# @use` package resolution
# (src/ModuleResolver.sa) reads package.qon / package.json / requirements.txt.
list(APPEND QUANTUM_SOURCES
    "${CMAKE_SOURCE_DIR}/src/qpm/QpmJson.sa"
    "${CMAKE_SOURCE_DIR}/src/qpm/QpmManifest.sa"
    "${CMAKE_SOURCE_DIR}/src/qpm/QpmSemver.sa")

# The Java prelude and the library headers (lib/cpp, lib/native) are carried as
# source: src/generated/*.sa, written by src/tools/embed.sa (picked up above).

# ── quantum  — compiler + bundler ─────────────────────────────────────────────
add_executable(quantum ${QUANTUM_SOURCES})
target_compile_definitions(quantum PRIVATE QUANTUM_MODE_COMPILER=1)
if(WIN32)
    target_link_libraries(quantum advapi32)
endif()

# ── qrun  — pure interpreter ──────────────────────────────────────────────────
add_executable(qrun ${QUANTUM_SOURCES})
target_compile_definitions(qrun PRIVATE QRUN_MODE=1)
if(WIN32)
    target_link_libraries(qrun advapi32)
endif()

# ── quantum_stub  — standalone runtime template ───────────────────────────────
add_executable(quantum_stub ${QUANTUM_SOURCES})
if(WIN32)
    target_link_libraries(quantum_stub advapi32)
endif()

# ── qpm  — standalone npm-compatible package manager ──────────────────────────
set(QPM_SOURCES ${CPP_SA})
list(FILTER QPM_SOURCES INCLUDE REGEX ".*/src/qpm/.*[.]sa$")
add_executable(qpm ${QPM_SOURCES})
if(WIN32)
    # z: zlib (statically linked via the -static link flag above, same as the
    #    other 3 targets already get a DLL-free libstdc++/libgcc).
    # winhttp/crypt32/ws2_32/shell32: Windows system DLLs, present on every
    #    install — linked the same way `advapi32` already is above.
    target_link_libraries(qpm z winhttp crypt32 ws2_32 shell32)
endif()

message(STATUS "Quantum v2.0.0 — Bytecode VM  (static linking enabled)")
message(STATUS "  Binaries : quantum  qrun  quantum_stub  qpm")