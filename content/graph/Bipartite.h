/**
 * Author: Team
 * License: CC0
 * Description: 2-colouring of an undirected graph (any number
 * of components). Returns colour 0/1 per node, or an empty
 * vector if some cycle is odd (not bipartite).
 * Usage: vi col = bipartite(g); if (col.empty()) ...
 * Time: O(V + E)
 * Status: stress-tested
 */
#pragma once

vi bipartite(const vector<vi>& g) {
	vi c(sz(g), -1);
	for (int s = 0; s < sz(g); s++) if (c[s] < 0) {
		vi q = {s};
		c[s] = 0;
		for (int i = 0; i < sz(q); i++) for (int v : g[q[i]]) {
			if (c[v] < 0) c[v] = c[q[i]] ^ 1, q.push_back(v);
			else if (c[v] == c[q[i]]) return {};
		}
	}
	return c;
}
