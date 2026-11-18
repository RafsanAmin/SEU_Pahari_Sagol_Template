/**
 * Author: Simon Lindholm (KACTL CompressTree), adapted to LCA.h
 * License: CC0
 * Description: Virtual (auxiliary) tree of a node subset: the
 * subset plus all pairwise LCAs, at most 2k - 1 nodes. Returns
 * (parent index in the list, node) pairs; entry 0 is the root
 * with parent 0. Edge lengths: L.dep[node] - L.dep[parent].
 * Typical use: tree DP over k marked nodes in O(k log N).
 * Usage: LCA L(g); auto vt = virtualTree(L, nodes);
 * Time: O(k \log k + k \log N)
 * Status: stress-tested
 */
#pragma once

#include "LCA.h"

vector<pii> virtualTree(LCA& L, vi li) {
	auto cmp = [&](int a, int b) { return L.tin[a] < L.tin[b]; };
	sort(all(li), cmp);
	for (int i = 0, m = sz(li) - 1; i < m; i++)
		li.push_back(L.lca(li[i], li[i + 1]));
	sort(all(li), cmp);
	li.erase(unique(all(li)), li.end());
	map<int, int> rev;
	for (int i = 0; i < sz(li); i++) rev[li[i]] = i;
	vector<pii> ret = {{0, li[0]}};
	for (int i = 0; i + 1 < sz(li); i++)
		ret.push_back({rev[L.lca(li[i], li[i + 1])], li[i + 1]});
	return ret;
}
