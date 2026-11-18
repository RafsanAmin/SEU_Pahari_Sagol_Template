/**
 * Author: Team
 * License: CC0
 * Description: Unweighted shortest paths from one or more
 * sources (-1 = unreachable). bfs01: weights 0/1 with a deque,
 * g[u] = \{(v, w)\}. Grid: move with dx/dy (first 4 = sides, all
 * 8 = with diagonals), check bounds, flatten (r, c) -> r * m + c.
 * Usage: vector<vi> g(n); vi d = bfs(g, {s});  // or {s1, s2, ...}
 * vector<vector<pii>> g2(n); g2[u].push_back({v, w}); bfs01(g2, s);
 * Time: O(V + E)
 * Status: stress-tested
 */
#pragma once

const int dx[] = {1, -1, 0, 0, 1, 1, -1, -1};
const int dy[] = {0, 0, 1, -1, 1, -1, 1, -1};
vi bfs(const vector<vi>& g, const vi& src) {
	vi d(sz(g), -1), q;
	for (int s : src) if (d[s] < 0) d[s] = 0, q.push_back(s);
	for (int i = 0; i < sz(q); i++)
		for (int v : g[q[i]]) if (d[v] < 0)
			d[v] = d[q[i]] + 1, q.push_back(v);
	return d;
}
vll bfs01(const vector<vector<pii>>& g, int s) {
	vll d(sz(g), INF);
	deque<int> q = {s};
	d[s] = 0;
	while (!q.empty()) {
		int u = q.front(); q.pop_front();
		for (auto [v, w] : g[u]) if (d[u] + w < d[v]) {
			d[v] = d[u] + w;
			w ? q.push_back(v) : q.push_front(v);
		}
	}
	return d;
}
