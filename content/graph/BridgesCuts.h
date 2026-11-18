/**
 * Author: Team
 * License: CC0
 * Description: Bridges and articulation points of an
 * undirected graph. g[u] = \{(v, edge id)\}, both directions
 * share the id (multi-edges are fine). Remove the bridges to
 * get the 2-edge-connected components.
 * Usage: vector<vector<pii>> g(n);  // edge id i between u and v:
 * g[u].push_back({v, i}); g[v].push_back({u, i});
 * LowLink L(g); L.bridges;  // edge ids; L.isCut[v]
 * Time: O(V + E)
 * Status: stress-tested
 */
#pragma once

struct LowLink {
	const vector<vector<pii>>& g;
	int timer = 0; vi tin, low, bridges; vector<bool> isCut;
	LowLink(const vector<vector<pii>>& g) : g(g),
			tin(sz(g), -1), low(sz(g)), isCut(sz(g)) {
		for (int i = 0; i < sz(g); i++) if (tin[i] < 0) dfs(i, -1);
	}
	void dfs(int u, int pe) {
		tin[u] = low[u] = timer++;
		int kids = 0;
		for (auto [v, id] : g[u]) if (id != pe) {
			if (tin[v] >= 0) {
				low[u] = min(low[u], tin[v]);
				continue;
			}
			dfs(v, id), kids++;
			low[u] = min(low[u], low[v]);
			if (low[v] > tin[u]) bridges.push_back(id);
			if (pe != -1 && low[v] >= tin[u]) isCut[u] = true;
		}
		if (pe == -1 && kids > 1) isCut[u] = true;
	}
};
