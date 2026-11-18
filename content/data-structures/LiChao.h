/**
 * Author: Team
 * License: CC0
 * Description: Li Chao tree: add lines kx + m, query the
 * minimum at integer x in [0, n-1]. For maximum, negate.
 * For a large x range, compress the query xs: use xs[x].
 * Usage: LiChao lc(n); lc.add(k, m); lc.query(x);
 * Time: O(\log N) per operation.
 * Status: stress-tested
 */
#pragma once

struct LiChao {
	struct Line {
		ll k, m;
		ll operator()(ll x) const { return k * x + m; }
	};
	int n; vector<Line> t;
	LiChao(int n) : n(n), t(4 * n, {0, INF}) {}
	void add(Line ln, int v, int l, int r) {
		int m = (l + r) / 2;
		bool lef = ln(l) < t[v](l), mid = ln(m) < t[v](m);
		if (mid) swap(t[v], ln);
		if (l == r) return;
		if (lef != mid) add(ln, 2*v, l, m);
		else add(ln, 2*v+1, m+1, r);
	}
	ll query(int x, int v, int l, int r) {
		if (l == r) return t[v](x);
		int m = (l + r) / 2;
		ll sub = x <= m ? query(x, 2*v, l, m)
		                : query(x, 2*v+1, m+1, r);
		return min(t[v](x), sub);
	}
	void add(ll k, ll m) { add({k, m}, 1, 0, n - 1); }
	ll query(int x) { return query(x, 1, 0, n - 1); }
};
