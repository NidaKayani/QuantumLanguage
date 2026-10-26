#!/usr/bin/env bash
# The native fixed point: qrun (whichever built it) compiles qrun_all.sa to a
# native qrun1.exe; qrun1 compiles the same source to qrun2, qrun2 to qrun3.
# All three come from the same code generator run on the same input, so they
# must be byte-identical. Then the native qrun builds quantum.exe,
# quantum_stub.exe and qpm.exe.
#   bash src/tools/native_bootstrap.sh            (QRUN=... to pick the first compiler)
set -e
cd "$(dirname "$0")/../.."
Q=${QRUN:-build/qrun.exe}
OUT=build/nboot
mkdir -p "$OUT"

step() {
    local compiler=$1 src=$2 out=$3
    local start=$(date +%s)
    "$compiler" --native "src/tools/selfhost/$src" "$OUT/$out"
    echo "  $out  ($(( $(date +%s) - start ))s, $(wc -c < "$OUT/$out") bytes, by $(basename "$compiler"))"
}

echo "native bootstrap: $Q"
step "$Q" qrun_all.sa qrun1.exe
step "$OUT/qrun1.exe" qrun_all.sa qrun2.exe
step "$OUT/qrun2.exe" qrun_all.sa qrun3.exe
if cmp -s "$OUT/qrun1.exe" "$OUT/qrun2.exe" && cmp -s "$OUT/qrun2.exe" "$OUT/qrun3.exe"; then
    echo "fixed point: qrun1 == qrun2 == qrun3"
else
    cmp "$OUT/qrun1.exe" "$OUT/qrun2.exe" || true
    cmp "$OUT/qrun2.exe" "$OUT/qrun3.exe" || true
    echo "NOT a fixed point"
    exit 1
fi
step "$OUT/qrun3.exe" quantum_all.sa quantum.exe
step "$OUT/qrun3.exe" stub_all.sa quantum_stub.exe
step "$OUT/qrun3.exe" qpm_all.sa qpm.exe
