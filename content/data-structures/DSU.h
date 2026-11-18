/**
 * Author: Team (adapted from KACTL)
 * License: CC0
 * Description: Disjoint set union with path compression and
 * union by size.
 * Usage: DSU d(n); d.unite(a, b); d.same(a, b); d.size(a);
 * Time: O(\alpha(N)) amortized per operation.
 * Status: stress-tested
 */
#pragma once

struct DSU {
	vi p;  // p[x] < 0: x is a root and -p[x] is its size
	DSU(int n) : p(n, -1) {}
	int find(int x) { return p[x] < 0 ? x : p[x] = find(p[x]); }
	int size(int x) { return -p[find(x)]; }
	bool same(int a, int b) { return find(a) == find(b); }
	bool unite(int a, int b) {
		a = find(a), b = find(b);
		if (a == b) return false;
		if (p[a] > p[b]) swap(a, b);
		p[a] += p[b], p[b] = a;
		return true;
	}
};
