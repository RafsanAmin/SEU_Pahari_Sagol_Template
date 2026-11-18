/**
 * Author: Team (adapted from KACTL)
 * License: CC0
 * Description: O(1) range query for an idempotent op
 * (min, max, gcd, and, or). Static array only.
 * Usage: SparseTable st(a); st.query(l, r);  // op set in EDIT
 * Time: O(N \log N) build, O(1) query.
 * Status: stress-tested
 */
#pragma once

struct SparseTable {
	// ---- EDIT HERE ----
	using T = ll;
	static T op(T a, T b) { return min(a, b); }
	// -------------------
	vector<vector<T>> t;
	SparseTable(const vector<T>& a) : t(1, a) {
		for (int k = 1; (1 << k) <= sz(a); k++) {
			t.emplace_back(sz(a) - (1 << k) + 1);
			for (int i = 0; i < sz(t[k]); i++)
				t[k][i] = op(t[k-1][i], t[k-1][i + (1 << (k-1))]);
		}
	}
	T query(int l, int r) {
		int k = __lg(r - l + 1);
		return op(t[k][l], t[k][r - (1 << k) + 1]);
	}
};
