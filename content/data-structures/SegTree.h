/**
 * Author: Team (adapted from KACTL)
 * License: CC0
 * Description: Point assign, range query, any associative merge.
 * Only edit the EDIT block. Node() must be the identity.
 * find\_first(l, r, p): first i in [l, r] with p(merge(a[l..i]))
 * true, -1 if none. p must be monotone in i (sum >= k, mx >= x).
 * Usage: SegTree st(a); st.update(i, x); st.query(l, r).sum;
 * st.find_first(l, r, [\&](auto\& nd) { return nd.sum >= k; });
 * Time: O(\log N) per operation, O(N) build.
 * Status: stress-tested
 */
#pragma once

struct SegTree {
	// ---- EDIT HERE ----
	using T = ll;
	struct Node { ll sum = 0; };  // Node() = identity
	static Node leaf(T x) { return {x}; }
	static Node merge(const Node& a, const Node& b) {
		return {a.sum + b.sum};
	}
	// -------------------
	int n; vector<Node> t;
	SegTree(int n) : SegTree(vector<T>(n)) {}
	SegTree(const vector<T>& a) : n(sz(a)), t(4 * n) {
		build(a, 1, 0, n - 1);
	}
	void pull(int v) { t[v] = merge(t[2*v], t[2*v+1]); }
	void build(const vector<T>& a, int v, int l, int r) {
		if (l == r) { t[v] = leaf(a[l]); return; }
		int m = (l + r) / 2;
		build(a, 2*v, l, m), build(a, 2*v+1, m+1, r);
		pull(v);
	}
	void update(int i, T x, int v, int l, int r) {
		if (l == r) { t[v] = leaf(x); return; }
		int m = (l + r) / 2;
		if (i <= m) update(i, x, 2*v, l, m);
		else update(i, x, 2*v+1, m+1, r);
		pull(v);
	}
	Node query(int ql, int qr, int v, int l, int r) {
		if (qr < l || r < ql) return Node();
		if (ql <= l && r <= qr) return t[v];
		int m = (l + r) / 2;
		return merge(query(ql, qr, 2*v, l, m),
		             query(ql, qr, 2*v+1, m+1, r));
	}
	template<class F> int find_first(int ql, int qr, F& p,
			Node& acc, int v, int l, int r) {
		if (qr < l || r < ql) return -1;
		if (ql <= l && r <= qr) {
			Node cur = merge(acc, t[v]);
			if (!p(cur)) { acc = cur; return -1; }
			if (l == r) return l;
		}
		int m = (l + r) / 2;
		int res = find_first(ql, qr, p, acc, 2*v, l, m);
		if (res != -1) return res;
		return find_first(ql, qr, p, acc, 2*v+1, m+1, r);
	}
	void update(int i, T x) { update(i, x, 1, 0, n - 1); }
	Node query(int l, int r) { return query(l, r, 1, 0, n-1); }
	template<class F> int find_first(int l, int r, F p) {
		Node acc;
		return find_first(l, r, p, acc, 1, 0, n - 1);
	}
};
