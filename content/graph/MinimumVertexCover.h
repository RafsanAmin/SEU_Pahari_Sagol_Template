/**
 * Author: Johan Sannemo, Simon Lindholm
 * Date: 2016-12-15
 * License: CC0
 * Description: Finds a minimum vertex cover in a bipartite graph.
 *  The size is the same as the size of a maximum matching, and
 *  the complement is a maximum independent set.
 * Usage: vi c = cover(g, n, m);  // g: left node -> right nodes
 * // right node v appears as n + v
 * Time: O(VE)
 * Status: stress-tested
 */
#pragma once

#include "DFSMatching.h"

vi cover(vector<vi>& g, int n, int m) {
	vi match(m, -1);
	int res = dfsMatching(g, match);
	vector<bool> lfound(n, true), seen(m);
	for (int it : match) if (it != -1) lfound[it] = false;
	vi q, cover;
	for (int i = 0; i < n; i++) if (lfound[i]) q.push_back(i);
	while (!q.empty()) {
		int i = q.back(); q.pop_back();
		lfound[i] = 1;
		for (int e : g[i]) if (!seen[e] && match[e] != -1) {
			seen[e] = true;
			q.push_back(match[e]);
		}
	}
	for (int i = 0; i < n; i++)
		if (!lfound[i]) cover.push_back(i);
	for (int i = 0; i < m; i++)
		if (seen[i]) cover.push_back(n+i);
	assert(sz(cover) == res);
	return cover;
}
