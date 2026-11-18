# ICPC Team Notebook: Plan

Based on KACTL (CC0, free to adapt), rewritten so every snippet follows **one style**.
Target team level: pupil → specialist. Coverage goes from beginner topics up to upper-intermediate
(roughly CF 2000-2200), plus a few advanced staples. We leave out niche KACTL content that a team at
this level won't use during a contest.

### Decisions (locked)
- **Build:** LaTeX, the same as KACTL (`pdflatex`). Snippet headers use KACTL's comment format so we
  can reuse its preprocessor. Needs MiKTeX or TeX Live installed before Phase 5.
- **Ranges:** inclusive `[l, r]`, 0-indexed.
- **Size:** 25 pages max.
- **Loops:** plain `for` loops.

---

## 1. Style contract (the main thing KACTL is missing)

Every file follows these rules, so once you know one snippet you can read the others.

| Rule | Choice |
|---|---|
| Language | C++17 (GNU), `bits/stdc++.h`, `using namespace std;` |
| Shared aliases | `ll`, `ull`, `ld`, `pii`, `pll`, `vi`, `vll`, `all(x)`, `sz(x)`. Nothing else |
| Loops | Plain `for` loops (no `rep` macro), because it's easier to read under pressure |
| Indexing | **0-indexed everywhere** |
| Ranges | **Inclusive `[l, r]` everywhere** (segtree, BIT, sparse table, hashing, Mo, HLD…) |
| Data structures | Always a `struct` with a constructor `(int n)` and/or `(const vector<T>& a)` |
| Method names | `update(...)`, `query(...)`, `build(...)` with the same meaning in every structure |
| Customisation | One clearly marked `// ---- EDIT HERE ----` block (Node / merge / identity / tag) |
| Graphs | `vector<vector<int>> g` (unweighted) or `vector<vector<pair<int,ll>>> g` (weighted), nodes `0..n-1` |
| Modular math | One shared `Mint` struct, reused by combinatorics, matrices and NTT |
| Header comment | 3 lines: **Description / Usage / Time**. Some files add a 4th line, **Tested on** |

### Example: segment tree family shares one skeleton
`SegTree`, `LazySegTree` and `PersistentSegTree` all use the same recursive layout
(`node, nl, nr`), the same `Node` + `merge` + identity, and the same `update`/`query` signatures.
`LazySegTree` only adds a `Tag` with `apply()` and `compose()`. Turning a normal segtree into a lazy
one means adding a tag, not learning a new implementation.

---

## 2. The "fill" utilities (requested)

```cpp
// fill_all(x, v): set every scalar inside x to v, for any depth.
// Works on C arrays, std::vector, std::array, and nested mixes (vector<array<int,5>>[10]...).
template<class T, class V> void fill_all(T& x, const V& v) {
    if constexpr (is_assignable_v<T&, const V&>) x = v;
    else for (auto& y : x) fill_all(y, v);
}
// make_vec<T>(d1, d2, ..., dk, init): k-dimensional vector filled with init.
// auto dp = make_vec<ll>(n, m, 2, -1LL);
```
These go in the main template, so they're on page 1.

---

## 3. Notebook outline

**0. Contest**: template.cpp (aliases, fill_all, make_vec, debug macro, fast IO), stress-test
script, hash-checker for typing code, troubleshooting checklist (WA/TLE/RE), useful pragmas.

**1. Math sheet**: formulas trimmed down from KACTL: sums, combinatorics identities, Catalan,
Stirling, Burnside, Pick, Euler's formula, probability and expectation tricks, primes table.

**2. Number theory**: binpow, gcd / extgcd, mod inverse, `Mint`, sieve (linear + SPF), divisors,
phi, factorisation (trial + Pollard rho), Miller–Rabin, CRT, Lucas, discrete log (BSGS).

**3. Combinatorics**: factorial/inverse precompute + nCr, Stirling/Catalan tables, derangements.

**4. Data structures**
- DSU (+ rollback variant)
- Fenwick: point/range, range-update range-query, 2D
- Sparse table (min/max/gcd)
- SegTree / LazySegTree / PersistentSegTree (shared skeleton, see §1)
- Ordered set (pbds), custom hash for `unordered_map`
- Implicit treap (split/merge, reverse)
- Mo's algorithm
- Li Chao tree + monotone CHT (LineContainer)
- Monotonic deque (sliding window min)

**5. Graphs**: BFS / 0-1 BFS / Dijkstra / Bellman-Ford / Floyd, topological sort, SCC (Tarjan) +
condensation, bridges & articulation points, 2-SAT, Kruskal MST, Euler path/circuit, Dinic max flow,
min-cost max-flow, Hopcroft–Karp bipartite matching.

**6. Trees**: LCA (binary lifting + Euler/sparse), tree diameter, rerooting DP template,
small-to-large, HLD (uses our SegTree), centroid decomposition.

**7. Strings**: prefix function (KMP), Z-function, polynomial hashing (mod 2^61−1, with range-hash
queries), Manacher, Trie (+ binary trie for XOR), Aho–Corasick, suffix array + LCP, suffix automaton.

**8. DP patterns**: LIS (n log n), knapsack variants, digit DP template, bitmask DP, SOS DP,
divide & conquer optimisation, Knuth notes, CHT/Li Chao cross-reference.

**9. Algebra / numerical**: matrix struct + fast exponentiation, Gaussian elimination
(mod p, double, and XOR/bitset), XOR linear basis, NTT convolution, FFT (double), ternary search.

**10. Geometry**: `Point` struct (templated on ll / ld), cross/dot/orientation, segment intersection,
point–segment distance, polygon area, point in polygon, convex hull, closest pair, circle basics.

**11. Misc**: binary search patterns (on answer, first true), coordinate compression, `mt19937`
random, bit tricks / `__builtin_*`, interactive-problem tips, int128 printing.

### Left out on purpose (too advanced or too niche for this level)
Link-cut tree, general (blossom) matching, min-cost circulation, simplex, polynomial
inverse/log/exp, Delaunay, 3D hull, suffix tree, dominator tree, segment-tree-beats.
We can add any of these later if there's space.

---

## 4. Verification (nothing goes in untested)

- Every snippet compiles with `g++ -std=c++17 -Wall -Wextra -D_GLIBCXX_DEBUG` (MinGW has no
  ASan, so we use libstdc++ debug mode to catch out-of-bounds access).
- Each DS, string and number-theory file gets a **stress test against a brute-force solution**
  in `tests/`. These tests aren't printed in the notebook.
- Where possible, a "Tested on" note points to a CSES or Library Checker problem.

---

## 5. Repo layout

```
content/<chapter>/*.h        # the snippets that get printed (KACTL header format)
content/<chapter>/*.md       # formula sheets / notes
tests/                       # brute-force stress tests (not printed)
build/                       # notebook generator -> notebook.pdf
STYLE.md                     # the style contract above
```

---

## 6. Phases

1. ✅ Template + fill utilities + STYLE.md + segment tree family
2. ✅ Other data structures + graphs + trees
3. ✅ Number theory + combinatorics + strings
4. ✅ DP + algebra + geometry + misc. Everything is stress-tested and passes `tests/lint.py`.
5. ⏳ Build the PDF (needs MiKTeX/TeX Live) and check the page count. Estimate: about half of
   KACTL's volume, so roughly 13–15 pages, which leaves room under the 25-page limit.

6. ✅ Tiers: core snippets (`\kactlimport`) go in `notebook.pdf`. Optional advanced ones
   (`\optimport`) are only in `library.pdf`. New core snippets: trie, merge sort tree, cycle finding,
   BFS / 0-1 BFS / grid, bipartite, Euler tour, diameter, dynamic segtree, weighted DSU, 2D prefix
   sums, monotonic queue/stack, segmented sieve, inversions, min rotation, hull diameter, point in
   hull. There are about 70 optional snippets, mostly from KACTL and checked with KACTL's own stress
   tests (`tests/run_kactl.sh`).

Changes from the original outline: Fenwick2D, BinaryTrie, rerooting and digit DP were added. DSU
rollback, a string trie (Aho-Corasick covers it) and a separate 0-1 BFS (a note in Dijkstra)
were dropped.
