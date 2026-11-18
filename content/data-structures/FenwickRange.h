/**
 * Author: Team
 * License: CC0
 * Description: Range add, range sum, using two Fenwicks.
 * Usage: FenwickRange f(n); f.update(l, r, x); f.query(l, r);
 * Time: O(\log N) per operation.
 * Status: stress-tested
 */
#pragma once

#include "Fenwick.h"

struct FenwickRange {
	Fenwick b1, b2;
	FenwickRange(int n) : b1(n + 1), b2(n + 1) {}
	void update(int l, int r, ll x) {  // a[l..r] += x
		b1.update(l, x), b1.update(r + 1, -x);
		b2.update(l, x * l), b2.update(r + 1, -x * (r + 1));
	}
	ll pref(int i) {  // a[0] + ... + a[i]
		return b1.query(i) * (i + 1) - b2.query(i);
	}
	ll query(int l, int r) { return pref(r) - pref(l - 1); }
};
