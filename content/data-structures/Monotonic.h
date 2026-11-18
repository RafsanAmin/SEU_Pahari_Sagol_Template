/**
 * Author: Team
 * License: CC0
 * Description: Monotonic deque / stack. slidingMin(a, k)[i] =
 * min of a[i..i+k-1]. nextGreater(a)[i] = first j > i with
 * a[j] > a[i], or n. Flip comparisons for max / smaller, and
 * reverse the array for "previous" versions.
 * Usage: vll mins = slidingMin(a, k); vi ng = nextGreater(a);
 * Time: O(N)
 * Status: stress-tested
 */
#pragma once

vll slidingMin(const vll& a, int k) {
	deque<int> d; vll res;
	for (int i = 0; i < sz(a); i++) {
		while (!d.empty() && a[d.back()] >= a[i]) d.pop_back();
		d.push_back(i);
		if (d.front() <= i - k) d.pop_front();
		if (i >= k - 1) res.push_back(a[d.front()]);
	}
	return res;
}
vi nextGreater(const vll& a) {
	int n = sz(a);
	vi res(n, n), st;
	for (int i = 0; i < n; i++) {
		while (!st.empty() && a[st.back()] < a[i])
			res[st.back()] = i, st.pop_back();
		st.push_back(i);
	}
	return res;
}
