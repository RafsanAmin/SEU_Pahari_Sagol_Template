/**
 * Author: Simon Lindholm (KACTL)
 * License: CC0
 * Description: Chinese remainder theorem. crt(a, m, b, n)
 * returns x with x = a (mod m), x = b (mod n), and
 * 0 <= x < lcm(m, n), or -1 if there is no solution.
 * Needs $0 \le a < m$, $0 \le b < n$, $mn < 2^{62}$.
 * Usage: ll x = crt(2, 3, 3, 5);  // 8
 * Time: O(\log n)
 * Status: stress-tested
 */
#pragma once

#include "Euclid.h"

ll crt(ll a, ll m, ll b, ll n) {
	if (n > m) swap(a, b), swap(m, n);
	ll x, y, g = euclid(m, n, x, y);
	if ((a - b) % g != 0) return -1;
	x = (b - a) % n * x % n / g * m + a;
	return x < 0 ? x + m * n / g : x;
}
