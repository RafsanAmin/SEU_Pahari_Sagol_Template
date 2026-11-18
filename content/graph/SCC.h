/**
 * Author: Team
 * License: CC0
 * Description: Strongly connected components (Tarjan).
 * comp[v] in [0, cnt). Components come in reverse topological
 * order: an edge u -> v means comp[u] >= comp[v].
 * Condensation DAG: edge comp[u] -> comp[v] if they differ.
 * Usage: SCC s(g); s.comp[v]; s.cnt;
 * Time: O(V + E)
 * Status: stress-tested
 */
#pragma once

struct SCC {
	int cnt = 0, timer = 0;
	vi comp, low, tin, st;
	SCC(const vector<vi>& g)
			: comp(sz(g), -1), low(sz(g)), tin(sz(g), -1) {
		for (int i = 0; i < sz(g); i++) if (tin[i] < 0) dfs(g, i);
	}
	void dfs(const vector<vi>& g, int u) {
		tin[u] = low[u] = timer++, st.push_back(u);
		for (int v : g[u]) {
			if (tin[v] < 0) dfs(g, v), low[u] = min(low[u], low[v]);
			else if (comp[v] < 0) low[u] = min(low[u], tin[v]);
		}
		if (low[u] != tin[u]) return;
		int v;
		do v = st.back(), st.pop_back(), comp[v] = cnt;
		while (v != u);
		cnt++;
	}
};
