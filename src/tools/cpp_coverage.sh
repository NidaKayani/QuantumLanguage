#!/usr/bin/env bash
# How much of the compiler (src/, Quantum in C++ syntax) the typed front-end compiles per file:
#   src/tools/cpp_coverage.sh [dir] [outfile]
# Each translation unit gets `qrun --strict --check` (no fallback to the
# dialect pipeline); <outfile> lists "ok<TAB>path" or "fail<TAB>path<TAB>first error".
cd "$(dirname "$0")/../.."
DIR=${1:-src}
OUT=${2:-build/cpp-coverage.txt}
QRUN=${QRUN:-build/qrun.exe}
mkdir -p "$(dirname "$OUT")"
: > "$OUT"
ok=0
n=0
while IFS= read -r f; do
    n=$((n + 1))
    if err=$(timeout 300 "$QRUN" --strict --check "$f" 2>&1 >/dev/null) && [ -z "$err" ]; then
        ok=$((ok + 1))
        printf 'ok\t%s\n' "$f" >> "$OUT"
    else
        first=$(printf '%s\n' "$err" | tr -d '\r' | sed 's/\x1b\[[0-9;]*m//g' | grep -m1 -E 'error|terminate|what\(\)')
        printf 'fail\t%s\t%s\n' "$f" "${first:-timeout or crash}" >> "$OUT"
    fi
done < <(find "$DIR" -path "$DIR/tools" -prune -o -type f -name '*.sa' ! -name '*.h.sa' -print |
         xargs grep -l '^// quantum: cpp' | sort)
echo "C++ front-end coverage: $ok / $n"
