/**
 * Author: Simon Lindholm (KACTL)
 * License: CC0
 * Description: Euler path/cycle, directed or undirected.
 * g[u] = \{(v, edge id)\}; undirected edges appear in both lists
 * with the same id. Returns the nodes of the walk starting
 * at src, or empty if none exists. For a path, src must be an
 * odd-degree node (undirected) or have out = in + 1 (directed).
 * Usage: vector<vector<pii>> g(n); g[u].push_back({v, i});  // edge id i
 * // (undirected: also g[v].push_back({u, i}))
 * vi walk = eulerWalk(g, m, src);  // empty if none
 * Time: O(V + E)
 * Status: stress-tested
 */
#pragma once

vi eulerWalk(const vector<vector<pii>>& g, int m,
		int src = 0) {
	int n = sz(g);
	vi D(n), its(n), used(m), ret, s = {src};
	D[src]++;  // allow paths, not just cycles
	while (!s.empty()) {
		int x = s.back(), &it = its[x];
		if (it == sz(g[x])) {
			ret.push_back(x), s.pop_back();
			continue;
		}
		auto [y, e] = g[x][it++];
		if (!used[e]) D[x]--, D[y]++, used[e] = 1, s.push_back(y);
	}
	for (int x : D) if (x < 0 || sz(ret) != m + 1) return {};
	return {ret.rbegin(), ret.rend()};
}
