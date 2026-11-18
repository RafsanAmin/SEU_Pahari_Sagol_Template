/**
 * Author: Team (adapted from KACTL)
 * License: CC0
 * Description: Point add, range sum. lower\_bound(s): smallest i
 * with a[0] + ... + a[i] >= s (needs all a[i] >= 0), n if none.
 * Usage: Fenwick f(n); f.update(i, x);  // a[i] += x
 * f.query(l, r); f.query(i);  // a[0..i]; f.lower_bound(s)
 * Time: O(\log N) per operation.
 * Status: stress-tested
 */
#pragma once

struct Fenwick {
	int n; vll t;
	Fenwick(int n) : n(n), t(n + 1) {}
	void update(int i, ll x) {  // a[i] += x
		for (i++; i <= n; i += i & -i) t[i] += x;
	}
	ll query(int i) {  // a[0] + ... + a[i]
		ll s = 0;
		for (i++; i > 0; i -= i & -i) s += t[i];
		return s;
	}
	ll query(int l, int r) { return query(r) - query(l - 1); }
	int lower_bound(ll s) {
		int pos = 0;
		for (int pw = 1 << __lg(n); pw; pw >>= 1)
			if (pos + pw <= n && t[pos + pw] < s)
				pos += pw, s -= t[pos];
		return pos;
	}
};
