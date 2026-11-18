/**
 * Author: Team
 * License: CC0
 * Description: Min-cost max-flow with SPFA (negative costs are
 * fine, negative cycles are not). flow(s, t, k) stops after
 * k units. Returns \{flow, cost\}.
 * Usage: MCMF f(n); f.addEdge(u, v, cap, cost);
 * auto [flow, cost] = f.flow(s, t);
 * Time: O(F \cdot VE) worst case, fast in practice.
 * Status: stress-tested
 */
#pragma once

struct MCMF {
	struct E { int to; ll cap, cost; };
	int n; vector<E> e; vector<vi> g; vll d; vi pe;
	MCMF(int n) : n(n), g(n) {}
	void addEdge(int u, int v, ll cap, ll cost) {
		g[u].push_back(sz(e)), e.push_back({v, cap, cost});
		g[v].push_back(sz(e)), e.push_back({u, 0, -cost});
	}
	bool spfa(int s, int t) {
		d.assign(n, INF), pe.assign(n, -1);
		vector<bool> inq(n);
		queue<int> q; q.push(s), d[s] = 0;
		while (!q.empty()) {
			int u = q.front(); q.pop(), inq[u] = 0;
			for (int id : g[u]) {
				auto [v, cap, cost] = e[id];
				if (cap > 0 && d[u] + cost < d[v]) {
					d[v] = d[u] + cost, pe[v] = id;
					if (!inq[v]) inq[v] = 1, q.push(v);
				}
			}
		}
		return d[t] < INF;
	}
	pll flow(int s, int t, ll k = INF) {
		ll f = 0, c = 0;
		while (f < k && spfa(s, t)) {
			ll add = k - f;
			for (int v = t; v != s; v = e[pe[v] ^ 1].to)
				add = min(add, e[pe[v]].cap);
			for (int v = t; v != s; v = e[pe[v] ^ 1].to)
				e[pe[v]].cap -= add, e[pe[v] ^ 1].cap += add;
			f += add, c += add * d[t];
		}
		return {f, c};
	}
};
