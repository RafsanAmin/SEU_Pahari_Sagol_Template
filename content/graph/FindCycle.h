/**
 * Author: Team
 * License: CC0
 * Description: Finds any cycle, as its nodes in order (the
 * last node has an edge back to the first), or empty if the
 * graph is acyclic. Directed: g[u] = \{v\}. Undirected:
 * g[u] = \{(v, edge id)\}, both directions share the id, so
 * multi-edges and self-loops count as cycles.
 * Usage: vi c = cycleDirected(g); vi c2 = cycleUndirected(g2);
 * Time: O(V + E)
 * Status: stress-tested
 */
#pragma once

vi cycleDirected(const vector<vi>& g) {
	int n = sz(g);
	vi col(n), par(n, -1), cyc;
	auto dfs = [&](auto& f, int u) -> bool {
		col[u] = 1;
		for (int v : g[u]) {
			if (!col[v]) { par[v] = u; if (f(f, v)) return true; }
			else if (col[v] == 1) {
				for (int x = u; x != v; x = par[x]) cyc.push_back(x);
				cyc.push_back(v), reverse(all(cyc));
				return true;
			}
		}
		col[u] = 2;
		return false;
	};
	for (int i = 0; i < n; i++)
		if (!col[i] && dfs(dfs, i)) break;
	return cyc;
}
vi cycleUndirected(const vector<vector<pii>>& g) {
	int n = sz(g);
	vi col(n), par(n, -1), cyc;
	auto dfs = [&](auto& f, int u, int pe) -> bool {
		col[u] = 1;
		for (auto [v, id] : g[u]) if (id != pe) {
			if (!col[v]) {
				par[v] = u;
				if (f(f, v, id)) return true;
			}
			else if (col[v] == 1) {
				for (int x = u; x != v; x = par[x]) cyc.push_back(x);
				cyc.push_back(v), reverse(all(cyc));
				return true;
			}
		}
		col[u] = 2;
		return false;
	};
	for (int i = 0; i < n; i++)
		if (!col[i] && dfs(dfs, i, -1)) break;
	return cyc;
}
