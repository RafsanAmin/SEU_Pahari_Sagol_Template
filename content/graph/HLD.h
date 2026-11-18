/**
 * Author: Team (adapted from KACTL)
 * License: CC0
 * Description: Heavy-light decomposition. Node v sits at index
 * pos[v] of a SegTree/LazySegTree. path(u, v, op) calls
 * op(l, r) on O(log N) inclusive index ranges, in no fixed
 * order (merge must be commutative). edges = true: the value
 * of edge (par[v], v) is stored at pos[v].
 * Usage: HLD h(g); for v: b[h.pos[v]] = a[v]; SegTree st(b);
 * h.path(u, v, [\&](int l, int r) { res += st.query(l, r).sum; });
 * auto [l, r] = h.subtree(v);
 * Time: O(\log^2 N) per path query with a segment tree.
 * Status: stress-tested
 */
#pragma once

struct HLD {
	int timer = 0; bool edges;
	vector<vi> g; vi par, siz, dep, rt, pos;
	HLD(const vector<vi>& g_, bool edges = false, int root = 0)
			: edges(edges), g(g_), par(sz(g), -1), siz(sz(g), 1),
			  dep(sz(g)), rt(sz(g), root), pos(sz(g)) {
		dfsSz(root), dfsHld(root);
	}
	void dfsSz(int v) {
		for (int& u : g[v]) {
			g[u].erase(find(all(g[u]), v));
			par[u] = v, dep[u] = dep[v] + 1;
			dfsSz(u);
			siz[v] += siz[u];
			if (siz[u] > siz[g[v][0]]) swap(u, g[v][0]);
		}
	}
	void dfsHld(int v) {
		pos[v] = timer++;
		for (int u : g[v]) {
			rt[u] = (u == g[v][0] ? rt[v] : u);
			dfsHld(u);
		}
	}
	template<class F> void path(int u, int v, F op) {
		for (; rt[u] != rt[v]; v = par[rt[v]]) {
			if (dep[rt[u]] > dep[rt[v]]) swap(u, v);
			op(pos[rt[v]], pos[v]);
		}
		if (dep[u] > dep[v]) swap(u, v);
		if (pos[u] + edges <= pos[v]) op(pos[u] + edges, pos[v]);
	}
	int lca(int u, int v) {
		for (; rt[u] != rt[v]; v = par[rt[v]])
			if (dep[rt[u]] > dep[rt[v]]) swap(u, v);
		return dep[u] < dep[v] ? u : v;
	}
	pii subtree(int v) {
		return {pos[v] + edges, pos[v] + siz[v] - 1};
	}
};
