/**
 * Author: Team
 * License: CC0
 * Description: 2-SAT. Variable x is the literal x, its negation
 * is \textasciitilde{}x. either(a, b) adds clause (a or b).
 * After solve(), val[x] is a valid assignment.
 * Usage: TwoSat ts(n); ts.either(0, \tilde3); ts.implies(1, 2);
 * ts.setValue(\tilde4); bool ok = ts.solve();
 * Time: O(N + M)
 * Status: stress-tested
 */
#pragma once

#include "SCC.h"

struct TwoSat {
	int n; vector<vi> g; vector<bool> val;
	TwoSat(int n) : n(n), g(2 * n) {}
	int id(int x) { return x >= 0 ? 2 * x : 2 * ~x + 1; }
	void either(int a, int b) {
		a = id(a), b = id(b);
		g[a ^ 1].push_back(b), g[b ^ 1].push_back(a);
	}
	void implies(int a, int b) { either(~a, b); }
	void setValue(int a) { either(a, a); }
	bool solve() {
		SCC s(g); val.assign(n, false);
		for (int i = 0; i < n; i++) {
			if (s.comp[2*i] == s.comp[2*i+1]) return false;
			val[i] = s.comp[2*i] < s.comp[2*i+1];
		}
		return true;
	}
};
