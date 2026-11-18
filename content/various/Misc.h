/**
 * Author: Team
 * License: CC0
 * Description: Small tools: coordinate compression, binary
 * search on the answer, random numbers, \_\_int128 printing,
 * floor/ceil division for negative numbers.
 * Usage: vll vals = compress(a);  // a[i] -> index into vals
 * ll x = firstTrue(lo, hi, [\&](ll m) { return ok(m); });
 * rnd(1, 6); floorDiv(-7, 2); ceilDiv(7, 2); print128(v);
 * Status: stress-tested
 */
#pragma once

vll compress(vll& a) {  // returns sorted distinct values
	vll v = a;
	sort(all(v)), v.erase(unique(all(v)), v.end());
	for (ll& x : a) x = lower_bound(all(v), x) - v.begin();
	return v;
}
// smallest x in [lo, hi] with ok(x) true, hi + 1 if none.
// ok must be monotone: false ... false true ... true
template<class F> ll firstTrue(ll lo, ll hi, F ok) {
	hi++;
	while (lo < hi) {
		ll mid = lo + (hi - lo) / 2;
		ok(mid) ? hi = mid : lo = mid + 1;
	}
	return lo;
}
mt19937_64 rng(chrono::steady_clock::now()
	.time_since_epoch().count());
ll rnd(ll lo, ll hi) {
	return uniform_int_distribution<ll>(lo, hi)(rng);
}
void print128(__int128 x) {
	if (x < 0) putchar('-'), x = -x;
	if (x > 9) print128(x / 10);
	putchar('0' + int(x % 10));
}
ll floorDiv(ll a, ll b) {
	return a / b - ((a ^ b) < 0 && a % b);
}
ll ceilDiv(ll a, ll b) {
	return a / b + ((a ^ b) > 0 && a % b);
}
// Bits: __builtin_popcountll(x), __builtin_ctzll(x) (x != 0),
// __lg(x) = highest set bit, x & -x = lowest set bit.
