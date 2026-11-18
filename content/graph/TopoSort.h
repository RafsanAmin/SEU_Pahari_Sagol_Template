/**
 * Author: Team
 * License: CC0
 * Description: Topological order of a directed graph (Kahn).
 * If the result has fewer than n nodes there is a cycle.
 * For the lexicographically smallest order use a min-heap.
 * Usage: vi order = topoSort(g); bool cyclic = sz(order) < n;
 * Time: O(V + E)
 * Status: stress-tested
 */
#pragma once

vi topoSort(const vector<vi>& g) {
	int n = sz(g);
	vi indeg(n), q;
	for (auto& adj : g) for (int v : adj) indeg[v]++;
	for (int i = 0; i < n; i++) if (!indeg[i]) q.push_back(i);
	for (int i = 0; i < sz(q); i++)
		for (int v : g[q[i]]) if (!--indeg[v]) q.push_back(v);
	return q;
}
