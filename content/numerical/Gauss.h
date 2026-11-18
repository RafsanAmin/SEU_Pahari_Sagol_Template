/**
 * Author: Team
 * License: CC0
 * Description: Solves A x = b for T = double or Mint. a is the
 * n x (m+1) augmented matrix [A | b]. Returns 0 (no solution),
 * 1 (unique) or 2 (infinitely many); x gets one solution
 * (free variables = 0). Rank = number of pivots found.
 * Usage: vector<vector<double>> a(n, vector<double>(m + 1));  // [A|b]
 * vector<double> x; int k = gauss(a, x);  // 0 / 1 / 2 solutions
 * Time: O(n m \min(n, m))
 * Status: stress-tested
 */
#pragma once

#include "../number-theory/ModInt.h"

double mag(double x) { return fabs(x); }
double mag(Mint x) { return x.v != 0; }
template<class T>
int gauss(vector<vector<T>> a, vector<T>& x) {
	const double EPS = 1e-9;
	int n = sz(a), m = sz(a[0]) - 1;
	vi where(m, -1);
	for (int col = 0, row = 0; col < m && row < n; col++) {
		int sel = row;
		for (int i = row; i < n; i++)
			if (mag(a[i][col]) > mag(a[sel][col])) sel = i;
		if (mag(a[sel][col]) < EPS) continue;
		swap(a[sel], a[row]), where[col] = row;
		for (int i = 0; i < n; i++) if (i != row) {
			T c = a[i][col] / a[row][col];
			for (int j = col; j <= m; j++) a[i][j] -= a[row][j] * c;
		}
		row++;
	}
	x.assign(m, T(0));
	for (int j = 0; j < m; j++)
		if (where[j] != -1) x[j] = a[where[j]][m] / a[where[j]][j];
	for (int i = 0; i < n; i++) {
		T s = 0;
		for (int j = 0; j < m; j++) s += x[j] * a[i][j];
		if (mag(s - a[i][m]) > EPS) return 0;
	}
	for (int j = 0; j < m; j++) if (where[j] == -1) return 2;
	return 1;
}
