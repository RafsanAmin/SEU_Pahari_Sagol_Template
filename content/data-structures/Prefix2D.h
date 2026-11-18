/**
 * Author: Team
 * License: CC0
 * Description: 2D prefix sums: rectangle sum in O(1).
 * 2D difference array (offline rectangle add): d[x1][y1] += v,
 * d[x1][y2+1] -= v, d[x2+1][y1] -= v, d[x2+1][y2+1] += v,
 * then take prefix sums.
 * Usage: Prefix2D p(a); p.query(x1, y1, x2, y2);
 * Time: O(NM) build, O(1) query.
 * Status: stress-tested
 */
#pragma once

struct Prefix2D {
	vector<vll> s;
	Prefix2D(const vector<vll>& a)
			: s(sz(a) + 1, vll(sz(a[0]) + 1)) {
		for (int i = 0; i < sz(a); i++)
			for (int j = 0; j < sz(a[0]); j++)
				s[i+1][j+1] = a[i][j] + s[i][j+1] + s[i+1][j]
				            - s[i][j];
	}
	ll query(int x1, int y1, int x2, int y2) {
		return s[x2+1][y2+1] - s[x1][y2+1]
		     - s[x2+1][y1] + s[x1][y1];
	}
};
