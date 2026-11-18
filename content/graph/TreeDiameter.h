/**
 * Author: Team
 * License: CC0
 * Description: A longest path of an unweighted tree (two BFS).
 * Diameter length = size - 1. Centre(s) = the middle one or two
 * nodes of the path. For weighted trees run the same idea with
 * distances from a DFS.
 * Usage: vi path = diameter(g);
 * Time: O(N)
 * Status: stress-tested
 */
#pragma once

vi diameter(const vector<vi>& g) {
	vi par;
	auto far = [&](int s) {
		vi d(sz(g), -1), q = {s};
		par.assign(sz(g), -1), d[s] = 0;
		for (int i = 0; i < sz(q); i++) for (int v : g[q[i]])
			if (d[v] < 0)
				d[v] = d[q[i]] + 1, par[v] = q[i], q.push_back(v);
		return q.back();
	};
	int b = far(far(0));
	vi path;
	for (int v = b; v != -1; v = par[v]) path.push_back(v);
	return path;
}
