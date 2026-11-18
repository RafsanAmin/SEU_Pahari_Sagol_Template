#!/bin/bash
# Compiles and runs every tests/*.test.cpp with debug checks enabled
# (libstdc++ debug mode everywhere, plus ASan/UBSan on Linux/WSL).
# Usage: bash tests/run.sh [name-filter]
cd "$(dirname "$0")"
export ASAN_OPTIONS=detect_leaks=0  # contest code never frees (e.g. treap nodes)
mkdir -p bin
fail=0
for f in *.test.cpp; do
    [[ -n "$1" && "$f" != *"$1"* ]] && continue
    exe="bin/${f%.cpp}"
    dbg="-D_GLIBCXX_DEBUG"
    [[ "$(uname)" == Linux ]] && dbg="$dbg -fsanitize=address,undefined -fno-sanitize-recover"
    head -1 "$f" | grep -q "no-debug" && dbg="${dbg/-D_GLIBCXX_DEBUG/}"
    if ! g++ -std=c++17 -O1 -Wall -Wextra $dbg -o "$exe" "$f"; then
        echo "COMPILE ERROR: $f"; fail=1; continue
    fi
    if ! "./$exe"; then echo "RUNTIME FAIL: $f"; fail=1; fi
done
[[ $fail == 0 ]] && echo "ALL PASSED"
exit $fail
