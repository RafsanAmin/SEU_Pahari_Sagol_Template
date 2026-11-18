/**
 * Author: Team
 * License: CC0
 * Description: Shortest paths from s with negative weights.
 * Edges \{u, v, w\}. dist = INF if unreachable, -INF if a
 * negative cycle can reach the node.
 * Usage: vector<array<ll, 3>> e; e.push_back({u, v, w});
 * vll d = bellmanFord(n, e, s);
 * Time: O(VE)
 * Status: stress-tested
 */
#pragma once

vll bellmanFord(int n, const vector<array<ll, 3>>& e, int s) {
	vll d(n, INF); d[s] = 0;
	for (int it = 0; it < 2 * n; it++)
		for (auto [u, v, w] : e) if (d[u] != INF) {
			ll nd = d[u] == -INF ? -INF : d[u] + w;
			if (nd < d[v]) d[v] = it >= n - 1 ? -INF : nd;
		}
	return d;
}
