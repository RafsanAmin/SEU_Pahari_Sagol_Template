/**
 * Author: Team
 * License: CC0
 * Description: SegTree over a huge index range [lo, hi] (e.g.
 * values up to 1e18, online) that creates nodes on demand.
 * Same EDIT block as SegTree. Unset positions are identity.
 * Usage: DynamicSegTree st(0, 1e9); st.update(i, x);
 * st.query(l, r).sum;
 * Time: O(\log(hi - lo)) time and new nodes per update.
 * Status: stress-tested
 */
#pragma once

struct DynamicSegTree {
	// ---- EDIT HERE ----
	using T = ll;
	struct Node { ll sum = 0; };  // Node() = identity
	static Node leaf(T x) { return {x}; }
	static Node merge(const Node& a, const Node& b) {
		return {a.sum + b.sum};
	}
	// -------------------
	ll lo, hi; int root = 0;
	vector<Node> t{Node()}; vi L{0}, R{0};  // node 0 = null
	DynamicSegTree(ll lo, ll hi) : lo(lo), hi(hi) {}
	int update(int v, ll i, T x, ll l, ll r) {
		if (!v) {
			v = sz(t);
			t.push_back(Node()), L.push_back(0), R.push_back(0);
		}
		if (l == r) { t[v] = leaf(x); return v; }
		ll m = l + (r - l) / 2;
		if (i <= m) L[v] = update(L[v], i, x, l, m);
		else R[v] = update(R[v], i, x, m + 1, r);
		t[v] = merge(t[L[v]], t[R[v]]);
		return v;
	}
	Node query(int v, ll ql, ll qr, ll l, ll r) {
		if (!v || qr < l || r < ql) return Node();
		if (ql <= l && r <= qr) return t[v];
		ll m = l + (r - l) / 2;
		return merge(query(L[v], ql, qr, l, m),
		             query(R[v], ql, qr, m + 1, r));
	}
	void update(ll i, T x) { root = update(root, i, x, lo, hi); }
	Node query(ll l, ll r) { return query(root, l, r, lo, hi); }
};
