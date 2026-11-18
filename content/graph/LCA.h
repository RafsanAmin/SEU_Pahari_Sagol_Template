/**
 * Author: Team
 * License: CC0
 * Description: Binary lifting on a rooted tree: LCA, distance,
 * k-th ancestor (root if k > depth). Add a parallel table to
 * aggregate path max/min/sum. tin[v] = DFS entry time.
 * Usage: LCA L(g, root); L.lca(a, b); L.dist(a, b); L.kth(v, k);
 * Time: O(N \log N) build, O(\log N) query.
 * Status: stress-tested
 */
#pragma once

struct LCA {
	int LOG, timer = 0; vi dep, tin; vector<vi> up;
	LCA(const vector<vi>& g, int root = 0)
			: LOG(__lg(sz(g)) + 1), dep(sz(g)), tin(sz(g)),
			  up(LOG, vi(sz(g), root)) {
		dfs(g, root, root);
		for (int k = 1; k < LOG; k++)
			for (int v = 0; v < sz(g); v++)
				up[k][v] = up[k-1][up[k-1][v]];
	}
	void dfs(const vector<vi>& g, int u, int p) {
		up[0][u] = p, tin[u] = timer++;
		for (int v : g[u]) if (v != p)
			dep[v] = dep[u] + 1, dfs(g, v, u);
	}
	int kth(int v, int k) {
		for (int i = 0; i < LOG; i++) if (k >> i & 1) v = up[i][v];
		return v;
	}
	int lca(int a, int b) {
		if (dep[a] < dep[b]) swap(a, b);
		a = kth(a, dep[a] - dep[b]);
		if (a == b) return a;
		for (int i = LOG - 1; i >= 0; i--)
			if (up[i][a] != up[i][b]) a = up[i][a], b = up[i][b];
		return up[0][a];
	}
	int dist(int a, int b) {
		return dep[a] + dep[b] - 2 * dep[lca(a, b)];
	}
};
