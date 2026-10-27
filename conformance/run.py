#!/usr/bin/env python3
"""Conformance suite for the strict front-ends: `qrun --strict` vs the real compilers.

Layout (one folder per language):
    conformance/<lang>/<name>.<ext>            a valid program
    conformance/<lang>/<name>.stdin            optional standard input for it
    conformance/<lang>/<name>.expected         its output under gcc / g++ / javac
    conformance/<lang>/<name>.exit             its exit status, when not 0
    conformance/<lang>/<name>.exit             its exit status, when not 0
    conformance/<lang>/invalid/<name>.<ext>    a program the real compiler rejects

    lang: c (.c), cpp (.cpp), java (.java)

A valid program passes when `qrun --strict` compiles it and prints exactly the
expected output (and exits with the expected status); an invalid one passes
when `qrun --strict` rejects it at compile time.

usage:
    python conformance/run.py [lang ...]          check (default: all languages)
    python conformance/run.py --regen [lang ...]  rewrite *.expected using the
        real toolchain: gcc/g++ on PATH, and javac/java from JAVA_HOME or PATH
    options: --qrun PATH (default: build/qrun.exe next to this folder)
             -k SUBSTR   only programs whose name contains SUBSTR
             -v          show diffs for failures
"""
import argparse, difflib, os, shutil, subprocess, sys, tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
EXT = {"c": ".c", "cpp": ".cpp", "java": ".java"}
TIMEOUT = 20


def norm(text):
    return text.replace("\r\n", "\n")


def read(path):
    with open(path, encoding="utf-8", newline="") as f:
        return f.read()


def run(cmd, stdin_text, cwd=None):
    try:
        p = subprocess.run(cmd, input=stdin_text.encode("utf-8"), capture_output=True,
                           timeout=TIMEOUT, cwd=cwd)
        return p.returncode, norm(p.stdout.decode("utf-8", "replace")), norm(p.stderr.decode("utf-8", "replace"))
    except subprocess.TimeoutExpired:
        return None, "", "timeout"


def java_tool(name):
    home = os.environ.get("JAVA_HOME")
    if home:
        cand = os.path.join(home, "bin", name + (".exe" if os.name == "nt" else ""))
        if os.path.exists(cand):
            return cand
    return shutil.which(name)


def java_main_name(source):
    import re
    m = re.search(r"\bpublic\s+(?:final\s+|abstract\s+)*(?:class|interface|enum|record)\s+(\w+)", source)
    return m.group(1) if m else "Main"


def reference(lang, src, stdin_text):
    """(compiled?, exit code, stdout) of the real toolchain."""
    with tempfile.TemporaryDirectory() as tmp:
        if lang in ("c", "cpp"):
            exe = os.path.join(tmp, "prog.exe")
            # The compilers' own default standards (gcc 16: gnu23 / gnu++20).
            cc = ["gcc"] if lang == "c" else ["g++"]
            r = subprocess.run(cc + ["-O0", "-o", exe, src, "-lm"], capture_output=True)
            if r.returncode != 0:
                return False, None, norm(r.stderr.decode("utf-8", "replace"))
            code, out, _ = run([exe], stdin_text, cwd=os.path.dirname(src))
            return True, code, out
        javac, java = java_tool("javac"), java_tool("java")
        if not javac or not java:
            sys.exit("javac/java not found: set JAVA_HOME")
        source = read(src)
        name = java_main_name(source)
        target = os.path.join(tmp, name + ".java")
        with open(target, "w", encoding="utf-8", newline="") as f:
            f.write(source)
        # Preview features on: JDK 21 has Java 25's compact source files
        # (top-level `void main()`) only as a preview.
        r = subprocess.run([javac, "-encoding", "UTF-8", "--enable-preview", "--release", "21", "-d", tmp, target],
                           capture_output=True)
        if r.returncode != 0:
            return False, None, norm(r.stderr.decode("utf-8", "replace"))
        code, out, _ = run([java, "--enable-preview", "-Dfile.encoding=UTF-8", "-Dstdout.encoding=UTF-8", "-cp", tmp,
                            name], stdin_text, cwd=tmp)
        return True, code, out


def programs(lang, pattern):
    folder = os.path.join(HERE, lang)
    valid, invalid = [], []
    if os.path.isdir(folder):
        for n in sorted(os.listdir(folder)):
            if n.endswith(EXT[lang]) and pattern in n:
                valid.append(os.path.join(folder, n))
        inv = os.path.join(folder, "invalid")
        if os.path.isdir(inv):
            for n in sorted(os.listdir(inv)):
                if n.endswith(EXT[lang]) and pattern in n:
                    invalid.append(os.path.join(inv, n))
    return valid, invalid


def stdin_for(src):
    p = os.path.splitext(src)[0] + ".stdin"
    return read(p) if os.path.exists(p) else ""


def expected_path(src):
    return os.path.splitext(src)[0] + ".expected"


def exit_path(src):
    return os.path.splitext(src)[0] + ".exit"


def expected_exit(src):
    p = exit_path(src)
    return int(read(p).strip()) if os.path.exists(p) else 0


def regen(langs, pattern):
    for lang in langs:
        valid, invalid = programs(lang, pattern)
        for src in valid:
            ok, code, out = reference(lang, src, stdin_for(src))
            name = os.path.relpath(src, HERE)
            if not ok:
                print("REJECTED by the real compiler (move it to invalid/?):", name)
                print(out)
                continue
            with open(expected_path(src), "w", encoding="utf-8", newline="\n") as f:
                f.write(out)
            if code != 0:
                with open(exit_path(src), "w", encoding="utf-8", newline="\n") as f:
                    f.write("%d\n" % code)
            elif os.path.exists(exit_path(src)):
                os.remove(exit_path(src))
            print("wrote", os.path.relpath(expected_path(src), HERE))
        for src in invalid:
            ok, _, _ = reference(lang, src, "")
            if ok:
                print("ACCEPTED by the real compiler (not invalid):", os.path.relpath(src, HERE))


def check(langs, pattern, qrun, verbose):
    total = failed = 0
    for lang in langs:
        valid, invalid = programs(lang, pattern)
        for src in valid:
            total += 1
            name = os.path.relpath(src, HERE)
            exp_file = expected_path(src)
            if not os.path.exists(exp_file):
                print("SKIP (no .expected)", name)
                continue
            want, want_code = read(exp_file).replace("\r\n", "\n"), expected_exit(src)
            code, out, err = run([qrun, "--strict", os.path.basename(src)], stdin_for(src), cwd=os.path.dirname(src))
            if code == want_code and out == want:
                continue
            failed += 1
            print("FAIL", name, "(exit %s, want %s)" % (code, want_code))
            if verbose:
                if err.strip():
                    print("  stderr:", err.strip()[:1500].replace("\n", "\n          "))
                for line in difflib.unified_diff(want.split("\n"), out.split("\n"), "expected", "qrun", lineterm="", n=1):
                    print("   ", line)
        for src in invalid:
            total += 1
            name = os.path.relpath(src, HERE)
            code, out, err = run([qrun, "--strict", "--check", os.path.basename(src)], "", cwd=os.path.dirname(src))
            if code not in (0, None) and "error" in err:
                if verbose:
                    print("ok  ", name, "->", err.strip().split("\n")[0])
                continue
            failed += 1
            print("FAIL", name, "(accepted; should be a compile error)")
    print("%d/%d passed" % (total - failed, total))
    return failed == 0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("langs", nargs="*", default=list(EXT))
    ap.add_argument("--regen", action="store_true")
    ap.add_argument("--qrun", default=os.path.join(HERE, "..", "build", "qrun.exe"))
    ap.add_argument("-k", default="")
    ap.add_argument("-v", action="store_true")
    a = ap.parse_args()
    if a.regen:
        regen(a.langs, a.k)
        return
    sys.exit(0 if check(a.langs, a.k, os.path.abspath(a.qrun), a.v) else 1)


if __name__ == "__main__":
    main()
