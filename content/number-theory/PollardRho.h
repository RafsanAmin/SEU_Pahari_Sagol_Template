/**
 * Author: Team (adapted from KACTL)
 * License: CC0
 * Description: Deterministic Miller-Rabin for n < 7e18 and
 * Pollard rho factorization. factor(n) returns the prime
 * factors in any order, with repeats (12 -> \{2, 3, 2\}).
 * Usage: isPrime(n); auto f = factor(n); sort(all(f));
 * Time: O(n^{1/4}) per factor, isPrime about 7 modpows.
 * Status: stress-tested
 */
#pragma once

ull modmul(ull a, ull b, ull m) {
	return (unsigned __int128)a * b % m;
}
ull modpow(ull b, ull e, ull m) {
	ull r = 1;
	for (; e; b = modmul(b, b, m), e /= 2)
		if (e & 1) r = modmul(r, b, m);
	return r;
}
bool isPrime(ull n) {
	if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
	ull A[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022},
	    s = __builtin_ctzll(n - 1), d = n >> s;
	for (ull a : A) {
		ull p = modpow(a % n, d, n), i = s;
		while (p != 1 && p != n - 1 && a % n && i--)
			p = modmul(p, p, n);
		if (p != n - 1 && i != s) return 0;
	}
	return 1;
}
ull pollard(ull n) {
	ull x = 0, y = 0, t = 30, prd = 2, i = 1, q;
	auto f = [&](ull v) { return modmul(v, v, n) + i; };
	while (t++ % 40 || __gcd(prd, n) == 1) {
		if (x == y) x = ++i, y = f(x);
		if ((q = modmul(prd, max(x, y) - min(x, y), n))) prd = q;
		x = f(x), y = f(f(y));
	}
	return __gcd(prd, n);
}
vector<ull> factor(ull n) {
	if (n == 1) return {};
	if (isPrime(n)) return {n};
	ull x = pollard(n);
	auto l = factor(x), r = factor(n / x);
	l.insert(l.end(), all(r));
	return l;
}
