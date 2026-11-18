/**
 * Author: Team
 * License: CC0
 * Description: DSU with potentials: keeps pot(b) - pot(a) for
 * a and b in the same set. unite(a, b, w) adds the constraint
 * pot(b) - pot(a) = w and returns false if it contradicts the
 * known ones. Online bipartiteness / parity: use unite(a, b, 1)
 * and compare diff(a, b) \% 2 (or switch + to \^{}).
 * Usage: WeightedDSU d(n); d.unite(a, b, w); d.diff(a, b);
 * Time: O(\log N) amortized per operation.
 * Status: stress-tested
 */
#pragma once

struct WeightedDSU {
	vi p; vll d;  // d[x] = pot(x) - pot(p[x])
	WeightedDSU(int n) : p(n, -1), d(n) {}
	int find(int x) {
		if (p[x] < 0) return x;
		int r = find(p[x]);
		d[x] += d[p[x]];
		return p[x] = r;
	}
	ll pot(int x) { find(x); return d[x]; }  // vs its root
	bool same(int a, int b) { return find(a) == find(b); }
	ll diff(int a, int b) { return pot(b) - pot(a); }
	bool unite(int a, int b, ll w) {
		w += pot(a) - pot(b);
		a = find(a), b = find(b);
		if (a == b) return w == 0;
		if (p[a] > p[b]) swap(a, b), w = -w;
		p[a] += p[b], p[b] = a, d[b] = w;
		return true;
	}
};
