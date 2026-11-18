/**
 * Author: Team
 * License: CC0
 * Description: One layer of dp[i] = min over j < i of
 * prv[j] + cost(j, i) when the best j is non-decreasing in i
 * (true if cost satisfies the quadrangle inequality). Typical:
 * split a[0..n-1] into k groups; prv[j] = best for the first j
 * elements, cost(j, i) = cost of a[j..i-1].
 * Usage: for each layer: cur.assign(n + 1, INF);
 * compute(1, n, 0, n - 1); swap(prv, cur);
 * Time: O(N \log N) cost calls per layer.
 * Status: stress-tested
 */
#pragma once

vll prv, cur;
ll cost(int j, int i);  // EDIT
void compute(int l, int r, int optl, int optr) {
	if (l > r) return;
	int mid = (l + r) / 2;
	pair<ll, int> best = {INF, optl};
	for (int j = optl; j <= min(mid - 1, optr); j++)
		if (prv[j] < INF)
			best = min(best, {prv[j] + cost(j, mid), j});
	cur[mid] = best.first;
	compute(l, mid - 1, optl, best.second);
	compute(mid + 1, r, best.second, optr);
}
