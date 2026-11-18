#!/bin/bash
# Compiles every snippet on its own, pasted after the contest template, exactly as a
# teammate would use it. Catches missing includes and clashes with template names (INF...).
# Usage: bash tests/compile_all.sh
cd "$(dirname "$0")/.."
root=$(pwd -W 2>/dev/null || pwd)   # native g++ on Windows needs a Windows path
mkdir -p tests/bin
fails=0
for f in $(cd content && find . -name "*.h" ! -path "./tex/*" | sed 's|^\./||' | sort); do
	printf '#include "%s/tests/test.h"\n#include "%s/content/%s"\nint main() {}\n' \
		"$root" "$root" "$f" > tests/bin/standalone.cpp
	if ! g++ -std=gnu++17 -fsyntax-only -Wall tests/bin/standalone.cpp 2> tests/bin/standalone.log; then
		echo "FAIL $f: $(grep -m1 error tests/bin/standalone.log | sed 's|.*content/||')"
		fails=$((fails + 1))
	fi
done
echo "standalone compile failures: $fails"
exit $((fails > 0))
