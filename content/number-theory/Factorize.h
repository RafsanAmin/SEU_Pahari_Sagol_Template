/**
 * Author: Team
 * License: CC0
 * Description: Trial division for one number up to about
 * 1e12: (prime, exponent) pairs, sorted divisors, phi(n).
 * Usage: factorize(360);  // {(2,3),(3,2),(5,1)}
 * vll d = divisors(n);  // sorted; phiOf(n);
 * Time: O(\sqrt n)
 * Status: stress-tested
 */
#pragma once

vector<pair<ll, int>> factorize(ll n) {
	vector<pair<ll, int>> res;
	for (ll p = 2; p * p <= n; p++) if (n % p == 0) {
		int e = 0;
		while (n % p == 0) n /= p, e++;
		res.push_back({p, e});
	}
	if (n > 1) res.push_back({n, 1});
	return res;
}
vll divisors(ll n) {
	vll small, big;
	for (ll d = 1; d * d <= n; d++) if (n % d == 0) {
		small.push_back(d);
		if (d * d != n) big.push_back(n / d);
	}
	small.insert(small.end(), big.rbegin(), big.rend());
	return small;
}
ll phiOf(ll n) {
	ll r = n;
	for (auto [p, e] : factorize(n)) r -= r / p;
	return r;
}
