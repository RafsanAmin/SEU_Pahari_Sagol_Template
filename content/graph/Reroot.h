/**
 * Author: Team
 * License: CC0
 * Description: Rerooting DP: the answer for every root in O(N).
 * up(x) turns a child's value into its contribution through the
 * edge to its parent. merge must be associative and commutative
 * with identity id(). Example: sum of distances to all nodes.
 * Usage: vector<T> ans = reroot(g);  // ans[v].second here
 * Time: O(N)
 * Status: stress-tested
 */
#pragma once

// ---- EDIT HERE ----
using T = pll;  // (nodes below, sum of their depths)
T id() { return {0, 0}; }
T merge(T a, T b) {
	return {a.first + b.first, a.second + b.second};
}
T up(T x) { return {x.first + 1, x.second + x.first + 1}; }
// -------------------
vector<T> reroot(const vector<vi>& g) {
	int n = sz(g);
	vi order = {0}, par(n, -1);
	for (int i = 0; i < sz(order); i++)
		for (int v : g[order[i]]) if (v != par[order[i]])
			par[v] = order[i], order.push_back(v);
	vector<T> dp(n, id()), fromPar(n, id()), ans(n);
	for (int i = n - 1; i > 0; i--) {
		int v = order[i];
		dp[par[v]] = merge(dp[par[v]], up(dp[v]));
	}
	for (int u : order) {
		vi ch;
		for (int v : g[u]) if (v != par[u]) ch.push_back(v);
		int k = sz(ch);
		vector<T> pre(k + 1, fromPar[u]), suf(k + 1, id());
		for (int i = 0; i < k; i++)
			pre[i + 1] = merge(pre[i], up(dp[ch[i]]));
		for (int i = k - 1; i >= 0; i--)
			suf[i] = merge(up(dp[ch[i]]), suf[i + 1]);
		ans[u] = pre[k];
		for (int i = 0; i < k; i++)
			fromPar[ch[i]] = up(merge(pre[i], suf[i + 1]));
	}
	return ans;
}
