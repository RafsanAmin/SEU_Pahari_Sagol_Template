/**
 * Author: Team
 * License: CC0
 * Description: Aho-Corasick automaton over 'a'..'z'.
 * insert returns the end node of a pattern; after build(),
 * nxt is a full automaton and cnt[v] = number of patterns that
 * are suffixes of node v's string. Per-pattern counts: walk the
 * text adding hit[v]++, then for v in reversed order:
 * hit[link[v]] += hit[v]; the answer is hit[end of pattern].
 * Usage: AhoCorasick ac; int e = ac.insert(p); ac.build();
 * for (char c : t) v = ac.nxt[v][c - 'a'], total += ac.cnt[v];
 * Time: O(26 \cdot total length)
 * Status: stress-tested
 */
#pragma once

struct AhoCorasick {
	vector<array<int, 26>> nxt{{}}; vi link{0}, order;
	vll cnt{0};
	int insert(const string& s) {
		int v = 0;
		for (char ch : s) {
			int c = ch - 'a';
			if (!nxt[v][c]) {
				nxt[v][c] = sz(nxt);
				nxt.push_back({}), link.push_back(0), cnt.push_back(0);
			}
			v = nxt[v][c];
		}
		cnt[v]++;
		return v;
	}
	void build() {
		queue<int> q;
		for (int c = 0; c < 26; c++)
			if (nxt[0][c]) q.push(nxt[0][c]);
		while (!q.empty()) {
			int v = q.front(); q.pop();
			order.push_back(v), cnt[v] += cnt[link[v]];
			for (int c = 0; c < 26; c++) {
				int& u = nxt[v][c];
				if (u) link[u] = nxt[link[v]][c], q.push(u);
				else u = nxt[link[v]][c];
			}
		}
	}
};
