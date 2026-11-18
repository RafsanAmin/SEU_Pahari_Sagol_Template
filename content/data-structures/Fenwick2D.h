/**
 * Author: Team
 * License: CC0
 * Description: Point add, rectangle sum on an n x m grid.
 * Usage: Fenwick2D f(n, m); f.update(x, y, v);
 * f.query(x1, y1, x2, y2);  // sum over [x1..x2] x [y1..y2]
 * Time: O(\log N \log M) per operation.
 * Status: stress-tested
 */
#pragma once

struct Fenwick2D {
	int n, m; vector<vll> t;
	Fenwick2D(int n, int m) : n(n), m(m), t(n + 1, vll(m + 1)) {}
	void update(int x, int y, ll v) {
		for (int i = x + 1; i <= n; i += i & -i)
			for (int j = y + 1; j <= m; j += j & -j) t[i][j] += v;
	}
	ll query(int x, int y) {  // sum over [0..x] x [0..y]
		ll s = 0;
		for (int i = x + 1; i > 0; i -= i & -i)
			for (int j = y + 1; j > 0; j -= j & -j) s += t[i][j];
		return s;
	}
	ll query(int x1, int y1, int x2, int y2) {
		return query(x2, y2) - query(x1 - 1, y2)
		     - query(x2, y1 - 1) + query(x1 - 1, y1 - 1);
	}
};
