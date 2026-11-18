/**
 * Author: Team
 * License: CC0
 * Description: Flattens a rooted tree: the subtree of v is the
 * index range [tin[v], tout[v]] and order[tin[v]] = v, so
 * subtree queries become range queries (Fenwick / SegTree).
 * Path root-to-v sums: add +x at tin[v] and -x at tout[v] + 1,
 * then the prefix sum at tin[u] is the sum on the root-u path.
 * Usage: EulerTour et(g, root); st.query(et.tin[v], et.tout[v]);
 * Time: O(N)
 * Status: stress-tested
 */
#pragma once

struct EulerTour {
	vi tin, tout, order, par, dep;
	EulerTour(const vector<vi>& g, int root = 0) : tin(sz(g)),
			tout(sz(g)), par(sz(g), -1), dep(sz(g)) {
		dfs(g, root);
	}
	void dfs(const vector<vi>& g, int u) {
		tin[u] = sz(order), order.push_back(u);
		for (int v : g[u]) if (v != par[u])
			par[v] = u, dep[v] = dep[u] + 1, dfs(g, v);
		tout[u] = sz(order) - 1;
	}
	bool isAnc(int a, int b) {  // a ancestor of b (or a == b)
		return tin[a] <= tin[b] && tout[b] <= tout[a];
	}
};
