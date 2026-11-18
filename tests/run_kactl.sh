#!/bin/bash
# Runs KACTL's own stress tests (copied into tests/kactl/) against our copies of the snippets.
# Usage: bash tests/run_kactl.sh [name-filter]
cd "$(dirname "$0")/kactl"
mkdir -p ../bin/kactl
pass=0; failed=""
for f in */*.cpp; do
    [[ "$f" == utilities/* ]] && continue
    [[ -n "$1" && "$f" != *"$1"* ]] && continue
    exe="../bin/kactl/$(echo "${f%.cpp}" | tr / _)"
    if g++ -std=gnu++17 -O2 -o "$exe" "$f" 2> "$exe.log" && timeout 300 "$exe" > /dev/null 2>> "$exe.log"; then
        pass=$((pass + 1))
    else
        failed="$failed $f"
    fi
done
echo "KACTL tests passed: $pass"
[[ -z "$failed" ]] && echo "ALL PASSED" && exit 0
echo "FAILED:$failed (logs in tests/bin/kactl/*.log)"
exit 1
