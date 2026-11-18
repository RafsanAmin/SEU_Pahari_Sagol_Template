# Notebook Style Rules

Every snippet in `content/` follows these rules. If a snippet breaks one, fix the snippet.
`python tests/lint.py` checks the mechanical rules for you.

## Basics
- C++17, written against `content/contest/template.cpp`. The only shared names you can use are
  `ll ull ld pii pll vi vll all(x) sz(x) INF` (`INF = 1e18`).
- Indent with **tabs** and use plain `for` loops.
- **Printed lines are at most 63 characters wide**, counting a tab as 2. Longer lines wrap in the
  3-column PDF. lint.py enforces this.
- **0-indexed. Every range is inclusive `[l, r]`.**
- Graph nodes are `0..n-1`. Use `vector<vi> g` for unweighted graphs and
  `vector<vector<pair<int, ll>>> g` for weighted ones. For edge lists or algorithms that need edge
  ids, use `g[u] = {(v, edge id)}`.

## Data structures
- Always a `struct`. Constructors take `(int n)` (n copies of `T()`) and/or `(const vector<T>& a)`.
- Use the same method names everywhere: `update(...)` changes data, `query(...)` reads it, and
  `find_first(l, r, pred)` binary-searches.
- Put everything a user should customise in one marked block:
  ```cpp
  // ---- EDIT HERE ----
  using T = ll;                       // element type
  struct Node { ... };                // Node() must be the identity
  static Node leaf(T x);
  static Node merge(const Node&, const Node&);
  // lazy only: struct Tag; apply(Node&, const Tag&); compose(Tag& a, const Tag& b)
  // -------------------
  ```
  For functions with a callback-style hook (Mo's add/del/calc, D&C DP's cost), mark the line `// EDIT`.
- Recursive helpers take `(..., int v, int l, int r)`: node v covers `[l, r]`. Query bounds are
  called `ql, qr`. Public wrappers with the same name start the recursion from the root. Children
  are recomputed with `pull(v)`, and lazy tags go down with `push(v)`.

## File format (KACTL header, read by content/tex/preprocessor.py)
```cpp
/**
 * Author: Team (adapted from KACTL)
 * License: CC0
 * Description: What it does, conventions, gotchas.
 * Usage: One or two lines of real calling code.
 * Time: O(\log N)
 * Status: stress-tested
 */
#pragma once
```
- Allowed keys: Author, Date, Description, Source, Time, Memory, License, Status, Usage, Details.
  Any other `Word:` at the start of a header line is an error. Rephrase "Note: ..." as "Note that ...".
- `Description` is LaTeX text, so escape `\_ \& \% \# \{ \}`, write `\^{}` and `\textasciitilde{}`,
  and put math in `$...$`.
- `Usage` is code. Write `_ { }` raw (the preprocessor escapes them), but still write `\&`, and
  `\tilde` for `~`.
- `Time` may use `O(...)` freely, because its contents become math.

## Core vs optional
- **Core** (`\kactlimport`, printed in the contest notebook): follows every rule in this file.
- **Optional** (`\optimport`, library only): mostly KACTL files that keep KACTL's API and naming.
  Only the loops were converted, plus line widths and includes. If you promote one to core,
  restyle it first: inclusive ranges and an EDIT block.

## Testing
A snippet goes in the notebook only after `tests/<Topic>.test.cpp` passes a stress test against a
brute-force solution. `bash tests/run.sh` runs everything with `-D_GLIBCXX_DEBUG` (bounds-checked
STL). Start a test file with `// no-debug` if it uses pb_ds, which is very slow in debug mode.
