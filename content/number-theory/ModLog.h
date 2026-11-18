/**
 * Author: Bjorn Martinsson (KACTL)
 * License: CC0
 * Description: Discrete log (baby-step giant-step): smallest
 * x > 0 with a\^{}x = b (mod m), or -1. Works for any m (a and m
 * need not be coprime). modLog(a, 1, m) = order of a.
 * Time: O(\sqrt m)
 * Usage: ll x = modLog(a, b, m);  // a^x = b (mod m), -1 if none
 * Status: stress-tested
 */
#pragma once

ll modLog(ll a, ll b, ll m) {
	ll n = (ll)sqrt(m) + 1, e = 1, f = 1, j = 1;
	unordered_map<ll, ll> A;
	while (j <= n && (e = f = e * a % m) != b % m)
		A[e * b % m] = j++;
	if (e == b % m) return j;
	if (__gcd(m, e) == __gcd(m, b))
		for (int i = 2; i < n + 2; i++)
			if (A.count(e = e * f % m)) return n * i - A[e];
	return -1;
}
