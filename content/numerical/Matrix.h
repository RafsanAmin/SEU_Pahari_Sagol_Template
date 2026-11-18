/**
 * Author: Team
 * License: CC0
 * Description: Matrix product and power over Mint. Linear
 * recurrence $f_n = \sum c_i f_{n-i}$: companion matrix power.
 * For Fibonacci, [[1,1],[1,0]]\^{}n has F(n) at [0][1].
 * Usage: Mat a(n, vector<Mint>(n)); Mat b = matPow(a, k);
 * Time: O(N^3 \log k)
 * Status: stress-tested
 */
#pragma once

#include "../number-theory/ModInt.h"

using Mat = vector<vector<Mint>>;
Mat operator*(const Mat& a, const Mat& b) {
	int n = sz(a), m = sz(b[0]), k = sz(b);
	Mat c(n, vector<Mint>(m));
	for (int i = 0; i < n; i++)
		for (int t = 0; t < k; t++)
			for (int j = 0; j < m; j++) c[i][j] += a[i][t] * b[t][j];
	return c;
}
Mat matPow(Mat a, ll e) {
	int n = sz(a);
	Mat r(n, vector<Mint>(n));
	for (int i = 0; i < n; i++) r[i][i] = 1;
	for (; e; e >>= 1, a = a * a) if (e & 1) r = r * a;
	return r;
}
