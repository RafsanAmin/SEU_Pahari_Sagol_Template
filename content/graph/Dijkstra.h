/**
 * Author: Team
 * License: CC0
 * Description: Shortest paths from s, weights >= 0.
 * g[u] = \{(v, w)\}. dist = INF if unreachable, par = previous
 * node (-1 at s / unreachable). For 0/1 weights use a deque
 * (push\_front on w = 0) instead: 0-1 BFS in O(V + E).
 * Usage: vector<vector<pair<int, ll>>> g(n); g[u].push_back({v, w});
 * vi par; vll d = dijkstra(g, s, par);  // path: follow par from t
 * Time: O((V + E) \log V)
 * Status: stress-tested
 */
#pragma once

template<class G> vll dijkstra(const G& g, int s, vi& par) {
	vll d(sz(g), INF); par.assign(sz(g), -1);
	priority_queue<pll, vector<pll>, greater<pll>> pq;
	pq.push({d[s] = 0, s});
	while (!pq.empty()) {
		auto [du, u] = pq.top(); pq.pop();
		if (du != d[u]) continue;
		for (auto [v, w] : g[u]) if (du + w < d[v])
			par[v] = u, pq.push({d[v] = du + w, v});
	}
	return d;
}
