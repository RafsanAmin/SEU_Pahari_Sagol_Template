/**
 * Author: Team
 * License: CC0
 * Description: Every update returns a new root; old versions
 * stay valid. Same EDIT block as SegTree. Root 0 = empty
 * version (all identity). kth(u, v, k): first i where prefix
 * sum of (version u - version v) is > k (needs a count Node).
 * Usage: PersistentSegTree pst(m); vi root(n + 1);
 * root[i+1] = pst.update(root[i], a[i],
 *     pst.query(root[i], a[i], a[i]).sum + 1);
 * pst.kth(root[r+1], root[l], k); // k-th smallest of a[l..r]
 * Time: O(\log N) time and memory per update.
 * Status: stress-tested
 */
#pragma once

struct PersistentSegTree {
	// ---- EDIT HERE ----
	using T = ll;
	struct Node { ll sum = 0; };  // Node() = identity
	static Node leaf(T x) { return {x}; }
	static Node merge(const Node& a, const Node& b) {
		return {a.sum + b.sum};
	}
	// -------------------
	int n; vector<Node> t; vi L, R;  // node 0 = null
	PersistentSegTree(int n) : n(n), t(1), L(1), R(1) {}
	int make(Node x, int l, int r) {
		t.push_back(x), L.push_back(l), R.push_back(r);
		return sz(t) - 1;
	}
	int build(const vector<T>& a, int l, int r) {
		if (l == r) return make(leaf(a[l]), 0, 0);
		int m = (l + r) / 2;
		int x = build(a, l, m), y = build(a, m + 1, r);
		return make(merge(t[x], t[y]), x, y);
	}
	int update(int v, int i, T x, int l, int r) {
		if (l == r) return make(leaf(x), 0, 0);
		int m = (l + r) / 2, a = L[v], b = R[v];
		if (i <= m) a = update(a, i, x, l, m);
		else b = update(b, i, x, m + 1, r);
		return make(merge(t[a], t[b]), a, b);
	}
	Node query(int v, int ql, int qr, int l, int r) {
		if (!v || qr < l || r < ql) return Node();
		if (ql <= l && r <= qr) return t[v];
		int m = (l + r) / 2;
		return merge(query(L[v], ql, qr, l, m),
		             query(R[v], ql, qr, m + 1, r));
	}
	int build(const vector<T>& a) { return build(a, 0, n-1); }
	int update(int root, int i, T x) {
		return update(root, i, x, 0, n - 1);
	}
	Node query(int root, int l, int r) {
		return query(root, l, r, 0, n - 1);
	}
	int kth(int u, int v, ll k) {
		int l = 0, r = n - 1;
		while (l < r) {
			int m = (l + r) / 2;
			ll c = t[L[u]].sum - t[L[v]].sum;
			if (k < c) u = L[u], v = L[v], r = m;
			else k -= c, u = R[u], v = R[v], l = m + 1;
		}
		return l;
	}
};
