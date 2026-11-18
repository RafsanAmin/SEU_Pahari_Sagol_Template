/**
 * Author: chilli, Simon Lindholm (KACTL)
 * License: CC0
 * Description: Convolution mod 998244353: c[k] = sum a[i] b[k-i].
 * Inputs must be in [0, nttMod). Other NTT primes (same code):
 * (5 << 25) + 1, (7 << 26) + 1, (479 << 21) + 1 (same root).
 * Usage: vll c = conv(a, b);  // a[i], b[i] in [0, nttMod)
 * Time: O(N \log N)
 * Status: stress-tested
 */
#pragma once

#include "../number-theory/Euclid.h"

const ll nttMod = 998244353, nttRoot = 62;
void ntt(vll& a) {
	int n = sz(a), L = 31 - __builtin_clz(n);
	static vll rt(2, 1);
	for (static int k = 2, s = 2; k < n; k *= 2, s++) {
		rt.resize(n);
		ll z[] = {1, power(nttRoot, nttMod >> s, nttMod)};
		for (int i = k; i < 2 * k; i++)
			rt[i] = rt[i / 2] * z[i & 1] % nttMod;
	}
	vi rev(n);
	for (int i = 0; i < n; i++)
		rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
	for (int i = 0; i < n; i++)
		if (i < rev[i]) swap(a[i], a[rev[i]]);
	for (int k = 1; k < n; k *= 2)
		for (int i = 0; i < n; i += 2 * k)
			for (int j = 0; j < k; j++) {
				ll z = rt[j+k] * a[i+j+k] % nttMod, &ai = a[i+j];
				a[i + j + k] = ai - z + (z > ai ? nttMod : 0);
				ai += (ai + z >= nttMod ? z - nttMod : z);
			}
}
vll conv(const vll& a, const vll& b) {
	if (a.empty() || b.empty()) return {};
	int s = sz(a) + sz(b) - 1, B = 32 - __builtin_clz(s),
	    n = 1 << B;
	ll inv = power(n, nttMod - 2, nttMod);
	vll L(a), R(b), out(n);
	L.resize(n), R.resize(n);
	ntt(L), ntt(R);
	for (int i = 0; i < n; i++)
		out[-i & (n - 1)] = L[i] * R[i] % nttMod * inv % nttMod;
	ntt(out);
	return {out.begin(), out.begin() + s};
}
