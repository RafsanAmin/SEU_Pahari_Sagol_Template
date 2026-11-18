/**
 * Author: Team
 * License: CC0
 * Description: Number of pairs i < j with a[i] > a[j], by merge
 * sort (a is sorted afterwards). Equals the minimum number of
 * adjacent swaps needed to sort a.
 * Usage: ll inv = inversions(a);
 * Time: O(N \log N)
 * Status: stress-tested
 */
#pragma once

ll inversions(vll& a) {
	int n = sz(a);
	if (n < 2) return 0;
	vll l(a.begin(), a.begin() + n / 2);
	vll r(a.begin() + n / 2, a.end());
	ll res = inversions(l) + inversions(r);
	for (int i = 0, j = 0, k = 0; k < n; k++)
		if (j == sz(r) || (i < sz(l) && l[i] <= r[j]))
			a[k] = l[i++];
		else a[k] = r[j++], res += sz(l) - i;
	return res;
}
