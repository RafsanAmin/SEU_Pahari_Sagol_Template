/**
 * Author: Team
 * License: CC0
 * Description: Segment tree that stores the sorted values of
 * every node. query(l, r, x) = number of a[i] <= x with i in
 * [l, r]. kth(l, r, k) = k-th smallest (0-indexed) of a[l..r].
 * Static array only; PersistentSegTree does kth in $O(\log N)$.
 * Usage: MergeSortTree mt(a); mt.query(l, r, x); mt.kth(l, r, k);
 * Time: O(N \log N) build, O(\log^2 N) query, kth O(\log^3 N).
 * Status: stress-tested
 */
#pragma once

struct MergeSortTree {
	int n; vector<vll> t;
	MergeSortTree(const vll& a) : n(sz(a)), t(4 * n) {
		build(a, 1, 0, n - 1);
	}
	void build(const vll& a, int v, int l, int r) {
		if (l == r) { t[v] = {a[l]}; return; }
		int m = (l + r) / 2;
		build(a, 2*v, l, m), build(a, 2*v+1, m+1, r);
		merge(all(t[2*v]), all(t[2*v+1]), back_inserter(t[v]));
	}
	int query(int ql, int qr, ll x, int v, int l, int r) {
		if (qr < l || r < ql) return 0;
		if (ql <= l && r <= qr)
			return upper_bound(all(t[v]), x) - t[v].begin();
		int m = (l + r) / 2;
		return query(ql, qr, x, 2*v, l, m)
		     + query(ql, qr, x, 2*v+1, m+1, r);
	}
	int query(int l, int r, ll x) {
		return query(l, r, x, 1, 0, n - 1);
	}
	ll kth(int l, int r, int k) {
		ll lo = t[1].front(), hi = t[1].back();
		while (lo < hi) {
			ll mid = lo + (hi - lo) / 2;
			if (query(l, r, mid) > k) hi = mid;
			else lo = mid + 1;
		}
		return lo;
	}
};
