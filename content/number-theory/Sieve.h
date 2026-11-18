/**
 * Author: Team
 * License: CC0
 * Description: Linear sieve up to n: primes, smallest prime
 * factor, Euler phi and Mobius mu. factorSmall(x) gives
 * (prime, exponent) pairs for x <= n using spf.
 * Usage: sieve(1e6); primes; spf[x]; phi[x]; mu[x];
 * factorSmall(360);  // {(2,3),(3,2),(5,1)}
 * Time: O(N) sieve, O(\log x) factorSmall.
 * Status: stress-tested
 */
#pragma once

vi primes, spf, phi, mu;
void sieve(int n) {
	spf.assign(n + 1, 0), phi = mu = spf, primes.clear();
	phi[1] = mu[1] = 1;
	for (int i = 2; i <= n; i++) {
		if (!spf[i]) spf[i] = i, phi[i] = i - 1, mu[i] = -1,
			primes.push_back(i);
		for (int p : primes) {
			if (p > spf[i] || 1LL * i * p > n) break;
			spf[i * p] = p;
			if (p == spf[i]) phi[i * p] = phi[i] * p;
			else phi[i * p] = phi[i] * (p - 1), mu[i * p] = -mu[i];
		}
	}
}
vector<pii> factorSmall(int x) {
	vector<pii> res;
	while (x > 1) {
		int p = spf[x], e = 0;
		while (x % p == 0) x /= p, e++;
		res.push_back({p, e});
	}
	return res;
}
