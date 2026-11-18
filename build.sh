#!/bin/bash
# Builds the PDFs with pdflatex (TeX Live or MiKTeX), e.g. from WSL in the repo root.
# Needs python3, cpp and md5sum on PATH.
#   bash build.sh              notebook.pdf     (core only)
#   bash build.sh full         fullnotebook.pdf (core + \optimport, <= 25 pages)
#   bash build.sh library      library.pdf      (everything, including \advimport)
#   bash build.sh fast         1 pass (table of contents may be stale)
# Open the PDFs in the repo root: each run compiles in a fresh temporary folder, so a
# PDF viewer holding a file open can never block the build.
set -e
cd "$(dirname "$0")"
export TEXINPUTS=".:content/tex/:"
doc=notebook; runs=2
for arg in "$@"; do
	[[ "$arg" == library ]] && doc=library
	[[ "$arg" == full ]] && doc=fullnotebook
	[[ "$arg" == fast ]] && runs=1
done
mkdir -p build
out=$(mktemp -d build/run.XXXXXX)
trap 'rm -rf "$out"' EXIT
for ((i = 0; i < runs; i++)); do
	pdflatex -shell-escape -interaction=nonstopmode -halt-on-error \
		-output-directory "$out" "content/$doc.tex" </dev/null
done
cp "$out/$doc.log" "build/$doc.log"
if cp "$out/$doc.pdf" "$doc.pdf" 2>/dev/null; then
	echo "Built $doc.pdf"
else  # the PDF is open (locked) in a viewer
	cp "$out/$doc.pdf" "$doc-new.pdf"
	echo "$doc.pdf is open in a viewer: wrote $doc-new.pdf instead"
fi
