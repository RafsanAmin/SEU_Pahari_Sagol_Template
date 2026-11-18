/**
 * Author: Team
 * License: CC0
 * Description: Factorials and inverse factorials mod MOD for
 * nCr. lucas(n, k) works for huge n when MOD is a small prime
 * (call initFact(MOD - 1) first).
 * Usage: initFact(2e5);  // once, >= largest n
 * Mint w = C(n, k); catalan(n); lucas(n, k);
 * Time: O(N) init, O(1) per C.
 * Status: stress-tested
 */
#pragma once

#include "../number-theory/ModInt.h"

vector<Mint> fact, ifact;
void initFact(int n) {
	fact.assign(n + 1, 1), ifact.assign(n + 1, 1);
	for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i;
	ifact[n] = fact[n].inv();
	for (int i = n; i > 0; i--) ifact[i - 1] = ifact[i] * i;
}
Mint C(int n, int k) {
	if (k < 0 || k > n) return 0;
	return fact[n] * ifact[k] * ifact[n - k];
}
Mint catalan(int n) { return C(2 * n, n) / (n + 1); }
Mint lucas(ll n, ll k) {
	if (k == 0) return 1;
	return C(n % MOD, k % MOD) * lucas(n / MOD, k / MOD);
}
