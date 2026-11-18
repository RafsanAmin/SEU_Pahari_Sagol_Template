/**
 * Author: Chen Xing (KACTL)
 * License: CC0
 * Description: Maximum bipartite matching. g[u] = right
 * neighbours of left node u. r must be vi(rightSize, -1);
 * afterwards r[v] = left match of v or -1.
 * By Konig's theorem, in bipartite graphs min vertex cover =
 * max matching and max independent set = V - max matching.
 * Usage: vi r(m, -1); int k = hopcroftKarp(g, r);
 * Time: O(E \sqrt V)
 * Status: stress-tested
 */
#pragma once

int hopcroftKarp(const vector<vi>& g, vi& r) {
	int n = sz(g), res = 0;
	vi l(n, -1), q(n), d(n);
	auto dfs = [&](auto f, int u) -> bool {
		int t = exchange(d[u], 0) + 1;
		for (int v : g[u])
			if (r[v] == -1 || (d[r[v]] == t && f(f, r[v])))
				return l[u] = v, r[v] = u, 1;
		return 0;
	};
	for (int t = 0, f = 0;; t = f = 0, d.assign(n, 0)) {
		for (int i = 0; i < n; i++)
			if (l[i] == -1) q[t++] = i, d[i] = 1;
		for (int i = 0; i < t; i++) for (int v : g[q[i]]) {
			if (r[v] == -1) f = 1;
			else if (!d[r[v]]) d[r[v]] = d[q[i]] + 1, q[t++] = r[v];
		}
		if (!f) return res;
		for (int i = 0; i < n; i++)
			if (l[i] == -1) res += dfs(dfs, i);
	}
}
