/**
 * Author: Team
 * License: CC0
 * Description: Centroid decomposition. lvl[v] = depth of v in
 * the centroid tree (at most log N), cpar[v] = parent there.
 * The component of centroid c is everything reachable from c
 * through nodes x with lvl[x] > lvl[c]. Every tree path goes
 * through the centroid of minimum lvl on it, so count paths
 * through each c with a DFS from c inside its component.
 * Usage: Centroid cd(g); cd.lvl[v]; cd.cpar[v];
 * // for c: DFS from c through nodes x with cd.lvl[x] > cd.lvl[c]
 * Time: O(N \log N)
 * Status: stress-tested
 */
#pragma once

struct Centroid {
	const vector<vi>& g; vi sub, cpar, lvl;
	Centroid(const vector<vi>& g) : g(g), sub(sz(g)),
			cpar(sz(g), -1), lvl(sz(g), -1) {
		build(0, -1, 0);
	}
	int dfs(int u, int p) {
		sub[u] = 1;
		for (int v : g[u]) if (v != p && lvl[v] < 0)
			sub[u] += dfs(v, u);
		return sub[u];
	}
	int find(int u, int p, int tot) {
		for (int v : g[u])
			if (v != p && lvl[v] < 0 && 2 * sub[v] > tot)
				return find(v, u, tot);
		return u;
	}
	void build(int u, int p, int d) {
		int c = find(u, -1, dfs(u, -1));
		cpar[c] = p, lvl[c] = d;
		for (int v : g[c]) if (lvl[v] < 0) build(v, c, d + 1);
	}
};
