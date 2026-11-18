/**
 * Author: Team (adapted from KACTL)
 * License: CC0
 * Description: Argmax of a unimodal function: f increases then
 * decreases. Integer version: smallest maximizing i in [a, b]
 * when f strictly increases before it. For minimum, flip the
 * comparisons. Real version: 100 iterations, any precision.
 * Usage: int i = ternSearch(0, n - 1, [\&](int i) { return a[i]; });
 * Time: O(\log(b - a))
 * Status: stress-tested
 */
#pragma once

template<class F> int ternSearch(int a, int b, F f) {
	while (b - a >= 5) {
		int mid = (a + b) / 2;
		if (f(mid) < f(mid + 1)) a = mid;
		else b = mid + 1;
	}
	for (int i = a + 1; i <= b; i++) if (f(a) < f(i)) a = i;
	return a;
}
template<class F>
double ternSearchReal(double lo, double hi, F f) {
	for (int it = 0; it < 100; it++) {
		double m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
		if (f(m1) < f(m2)) lo = m1;
		else hi = m2;
	}
	return lo;
}
