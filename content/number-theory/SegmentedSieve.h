/**
 * Author: Team
 * License: CC0
 * Description: Primes in [L, R] for large R (about 1e12) when
 * R - L is small (about 1e7). isP[i] tells if L + i is prime.
 * Usage: auto isP = segmentedSieve(L, R);
 * Time: O((R - L) \log\log R + \sqrt R)
 * Status: stress-tested
 */
#pragma once

vector<bool> segmentedSieve(ll L, ll R) {
	ll lim = sqrtl(R) + 1;
	vector<bool> small(lim + 1, true), isP(R - L + 1, true);
	for (ll p = 2; p <= lim; p++) if (small[p]) {
		for (ll j = p * p; j <= lim; j += p) small[j] = false;
		for (ll j = max(p * p, (L + p - 1) / p * p); j <= R;
				j += p)
			isP[j - L] = false;
	}
	for (ll x = L; x <= min(R, 1LL); x++) isP[x - L] = false;
	return isP;
}
