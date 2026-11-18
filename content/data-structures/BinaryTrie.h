/**
 * Author: Team
 * License: CC0
 * Description: Multiset of B-bit numbers; max of x XOR y over
 * stored y. insert(x, -1) erases one copy of x.
 * Usage: BinaryTrie t; t.insert(x); t.insert(x, -1);  // erase
 * int best = t.maxXor(x);  // trie must be non-empty
 * Time: O(B) per operation.
 * Status: stress-tested
 */
#pragma once

struct BinaryTrie {
	static const int B = 30;
	vector<array<int, 2>> nxt{{0, 0}}; vi cnt{0};
	void insert(int x, int d = 1) {
		int v = 0;
		for (int b = B - 1; b >= 0; b--) {
			int c = x >> b & 1;
			if (!nxt[v][c]) {
				nxt[v][c] = sz(nxt);
				nxt.push_back({0, 0}), cnt.push_back(0);
			}
			v = nxt[v][c], cnt[v] += d;
		}
	}
	int maxXor(int x) {
		int v = 0, res = 0;
		for (int b = B - 1; b >= 0; b--) {
			int c = (x >> b & 1) ^ 1, u = nxt[v][c];
			if (u && cnt[u] > 0) v = u, res |= 1 << b;
			else v = nxt[v][c ^ 1];
		}
		return res;
	}
};
