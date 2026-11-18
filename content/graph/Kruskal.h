/**
 * Author: Team
 * License: CC0
 * Description: Minimum spanning tree (forest if disconnected).
 * Edges \{w, u, v\}. Returns the total weight.
 * Usage: vector<array<ll, 3>> e; e.push_back({w, u, v});
 * ll cost = kruskal(n, e);
 * Time: O(E \log E)
 * Status: stress-tested
 */
#pragma once

#include "../data-structures/DSU.h"

ll kruskal(int n, vector<array<ll, 3>> e) {
	sort(all(e));
	DSU d(n); ll res = 0;
	for (auto [w, u, v] : e) if (d.unite(u, v)) res += w;
	return res;
}
