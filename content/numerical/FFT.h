/**
 * Author: Ludo Pulles, chilli, Simon Lindholm (KACTL)
 * License: CC0
 * Description: Real convolution with doubles. Rounding is safe
 * if $(\sum a_i^2 + \sum b_i^2)\log_2 N < 9 \cdot 10^{14}$; round
 * results with llround. Otherwise use NTT.
 * Usage: vector<double> c = convDouble(a, b); ll ci = llround(c[i]);
 * Time: O(N \log N), about 1s for N = 2\^{}22.
 * Status: stress-tested
 */
#pragma once

typedef complex<double> C;
void fft(vector<C>& a) {
	int n = sz(a), L = 31 - __builtin_clz(n);
	static vector<complex<long double>> R(2, 1);
	static vector<C> rt(2, 1);
	for (static int k = 2; k < n; k *= 2) {
		R.resize(n), rt.resize(n);
		auto x = polar(1.0L, acos(-1.0L) / k);
		for (int i = k; i < 2 * k; i++)
			rt[i] = R[i] = i & 1 ? R[i / 2] * x : R[i / 2];
	}
	vi rev(n);
	for (int i = 0; i < n; i++)
		rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
	for (int i = 0; i < n; i++)
		if (i < rev[i]) swap(a[i], a[rev[i]]);
	for (int k = 1; k < n; k *= 2)
		for (int i = 0; i < n; i += 2 * k)
			for (int j = 0; j < k; j++) {
				C z = rt[j + k] * a[i + j + k];
				a[i + j + k] = a[i + j] - z;
				a[i + j] += z;
			}
}
vector<double> convDouble(const vector<double>& a,
		const vector<double>& b) {
	if (a.empty() || b.empty()) return {};
	vector<double> res(sz(a) + sz(b) - 1);
	int L = 32 - __builtin_clz(sz(res)), n = 1 << L;
	vector<C> in(n), out(n);
	copy(all(a), begin(in));
	for (int i = 0; i < sz(b); i++) in[i].imag(b[i]);
	fft(in);
	for (C& x : in) x *= x;
	for (int i = 0; i < n; i++)
		out[i] = in[-i & (n - 1)] - conj(in[i]);
	fft(out);
	for (int i = 0; i < sz(res); i++)
		res[i] = imag(out[i]) / (4 * n);
	return res;
}
