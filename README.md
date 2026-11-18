# ICPC Team Notebook

A KACTL-based team reference, rewritten so the core snippets follow one style
([STYLE.md](STYLE.md)): 0-indexed, inclusive `[l, r]` ranges, and one `// ---- EDIT HERE ----`
block per structure. Every snippet is stress-tested.

## Three PDFs from one source
| File | Contents | Pages |
|---|---|---|
| `content/notebook.tex` → **notebook.pdf** | core snippets (`\kactlimport`) | 19 |
| `content/fullnotebook.tex` → **fullnotebook.pdf** | core + optional (`\optimport`): **print this one for contests** | 24 (limit 25) |
| `content/library.tex` → **library.pdf** | everything, including over-advanced (`\advimport`) | 31 |

Each `content/<chapter>/chapter.tex` decides where a snippet goes:
```tex
\kactlimport{SegTree.h}       % in all three
\optimport{SegTreeBeats.h}    % fullnotebook + library
\advimport{LinkCutTree.h}     % library only (over-advanced for our level)
```
To move a snippet, change that one word. Rebuild `fullnotebook` and check it is still at most 25 pages.

## Contents
**Core** (contest notebook), from beginner up to roughly CF 2200:

| Chapter | Snippets |
|---|---|
| Contest | template (`fill_all`, `make_vec`, `dbg`), hash.sh, stress.sh, troubleshooting |
| Math | formula sheet: sums, triangles, probability, game theory |
| Data structures | DSU, weighted DSU, Fenwick (point / range-range / 2D), 2D prefix sums, sparse table, monotonic deque/stack, SegTree, LazySegTree, DynamicSegTree, PersistentSegTree, merge sort tree, ordered set, safe hash map, treap, Mo, LineContainer, Li Chao, XOR trie |
| Number theory | Mint, power/extgcd/inverse, CRT, discrete log, linear sieve, segmented sieve, trial factorisation + divisors, Miller-Rabin + Pollard rho, facts |
| Combinatorial | factorials, nCr, Catalan, Lucas, counting formulas |
| Graph | BFS (multi-source, 0-1, grid), Dijkstra, Bellman-Ford, Floyd, bipartite check, cycle finding (directed and undirected), topo sort, SCC, 2-SAT, bridges / cut points, Kruskal, Euler walk, Dinic, MCMF, Hopcroft-Karp, Euler tour, diameter, LCA, HLD, centroid decomposition, rerooting |
| Strings | KMP, Z, hashing, Manacher, trie, Aho-Corasick, min rotation, suffix array, suffix automaton |
| DP and misc | LIS, inversions, divide & conquer DP, SOS, digit DP, compression / binary search / bits |
| Algebra | matrix power, Gauss (double and mod), XOR basis, NTT, FFT, ternary search |
| Geometry | point primitives, segments, angle sort, circles, MEC, polygon area / inside, hull, hull diameter, point in hull, closest pair |

**Optional** (library only): rollback DSU, segment tree beats, mod sqrt, floor sum, fast
sieve, continued fractions, IntPerm, multinomial, biconnected components, directed MST, edge
colouring, fast MCMF, push-relabel, global min cut, Gomory-Hu, Kuhn, min vertex cover, Hungarian,
general matching, virtual tree, link-cut tree, cliques / independent set, palindromic tree,
suffix tree, Knuth DP, interval tools, fast knapsack, fast input, fast mod, bump allocator,
determinants, matrix inverse, binary systems, tridiagonal, FFT mod, AND/OR/XOR convolution,
Berlekamp-Massey, linear recurrence, polynomials, interpolation, golden section, hill climbing,
integration, simplex, BigInt, and extra geometry (line tools, tangents, circle ∩ polygon,
polygon cut / union / centre, line ∩ hull, Manhattan MST, kd-tree, Delaunay, 3D).

## Commands (Git Bash)
```bash
bash tests/run.sh          # our brute-force stress tests (about 2 min)
bash tests/run.sh Graph    # only test files whose name contains "Graph"
bash tests/run_kactl.sh    # KACTL's own stress tests, run against our copies
bash tests/compile_all.sh  # every snippet compiles alone after the template (no name clashes)
python tests/lint.py       # headers, LaTeX escaping, printed line width (<= 63)
bash build.sh              # notebook.pdf   (needs pdflatex: MiKTeX or TeX Live)
bash build.sh full         # fullnotebook.pdf
bash build.sh library      # library.pdf
```
The build calls `python3`, `sh`, `cpp` and `md5sum` from inside LaTeX (`-shell-escape`).

**Building with WSL (Ubuntu)**, which is how the PDFs here are built:
```bash
sudo apt install g++ make texlive-latex-extra texlive-fonts-recommended texlive-plain-generic
cd "/mnt/d/Rafsan Data/Coding/Team Competitions Practise/Team Template"
bash build.sh && bash build.sh library
```
Under Linux, `tests/run.sh` also turns on AddressSanitizer and UBSan.
`.gitattributes` keeps every file LF. A CRLF `hash.sh` breaks the build.

## Layout
```
content/notebook.tex, library.tex    top-level LaTeX (edit team / university names)
content/<chapter>/                   snippets (*.h) + chapter.tex (order and tier)
content/tex/                         KACTL's LaTeX style + preprocessor (+ \optimport)
tests/*.test.cpp                     our stress tests (not printed)
tests/kactl/                         KACTL's stress tests, rewired to our files
tests/lint.py, run.sh, run_kactl.sh, compile_all.sh   checks
tests/import_kactl.py                how the optional KACTL files were imported
```

## Adding a snippet
1. Write `content/<chapter>/Name.h` with the KACTL header (see STYLE.md).
2. Add `\kactlimport{Name.h}` or `\optimport{Name.h}` to that chapter's `chapter.tex`.
3. Add a brute-force test in `tests/`, then run `bash tests/run.sh` and `python tests/lint.py`.

Credits: many algorithms and the LaTeX build come from
[KACTL](https://github.com/kth-competitive-programming/kactl) (CC0).
