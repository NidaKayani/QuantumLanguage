#!/usr/bin/env bash
# The native backend's tests: each tests/native/<name>.cpp is compiled by
#   qrun --native <name>.cpp <name>.exe
# (no assembler, linker or C++ compiler), run, and its output plus exit code
# compared with <name>.expected, byte for byte.
#   src/tools/native_test.sh [outdir]
# SELFHOST=1 compiles with the self-hosted compiler instead
# (build/quantumc.qbc run on qrun). QRUN=path overrides the qrun used.
cd "$(dirname "$0")/../.."
OUT=${1:-build/native-test}
QRUN=${QRUN:-build/qrun.exe}
mkdir -p "$OUT"
if [ -n "$SELFHOST" ]; then
    "$QRUN" --emit-qbc src/tools/selfhost/quantumc.sa build/quantumc.qbc || exit 1
    COMPILE=("$QRUN" --exec build/quantumc.qbc --native)
else
    COMPILE=("$QRUN" --native)
fi
ok=0
n=0
for src in tests/native/*.cpp; do
    name=$(basename "$src" .cpp)
    n=$((n + 1))
    if ! "${COMPILE[@]}" "$src" "$OUT/$name.exe" > "$OUT/$name.compile.txt" 2>&1; then
        echo "FAIL $name: does not compile"
        sed 's/^/    /' "$OUT/$name.compile.txt"
        continue
    fi
    "$OUT/$name.exe" > "$OUT/$name.out" 2>&1
    echo "exit=$?" >> "$OUT/$name.out"
    if cmp -s "$OUT/$name.out" "tests/native/$name.expected"; then
        ok=$((ok + 1))
    else
        echo "FAIL $name: output differs"
        diff "tests/native/$name.expected" "$OUT/$name.out" | head -10 | sed 's/^/    /'
    fi
done
echo "native: $ok / $n"
[ "$ok" = "$n" ]
