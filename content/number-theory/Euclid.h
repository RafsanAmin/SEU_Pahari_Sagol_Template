/**
 * Author: Team (adapted from KACTL)
 * License: CC0
 * Description: power(b, e, m) = b\^{}e mod m. euclid = extended
 * Euclid (the "Pulverizer"): x, y with ax + by = gcd(a, b).
 * diophantine solves ax + by = c (a, b > 0): false if
 * gcd(a, b) does not divide c, else the solution with the
 * smallest x >= 0; all: x + k b/g, y - k a/g. modInverse works
 * for any m with gcd(a, m) = 1 (else -1). gcd: std::gcd.
 * Usage: ll x, y, g = euclid(a, b, x, y);
 * if (diophantine(a, b, c, x, y)) ...  // a*x + b*y == c
 * Time: O(\log)
 * Status: stress-tested
 */
#pragma once

ll power(ll b, ll e, ll m) {
	ll r = 1 % m;
	for (b %= m; e; e >>= 1, b = b * b % m)
		if (e & 1) r = r * b % m;
	return r;
}
ll euclid(ll a, ll b, ll& x, ll& y) {
	if (!b) return x = 1, y = 0, a;
	ll d = euclid(b, a % b, y, x);
	return y -= a / b * x, d;
}
bool diophantine(ll a, ll b, ll c, ll& x, ll& y) {
	ll g = euclid(a, b, x, y), m = b / g;
	if (c % g) return false;
	x = ll(((__int128)x * (c / g) % m + m) % m);
	y = ll(((__int128)c - (__int128)a * x) / b);
	return true;
}
ll modInverse(ll a, ll m) {
	ll x, y;
	if (euclid((a % m + m) % m, m, x, y) != 1) return -1;
	return (x % m + m) % m;
}
