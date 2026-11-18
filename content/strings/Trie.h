/**
 * Author: Team
 * License: CC0
 * Description: Trie over 'a'..'z'. pass[v] = number of words
 * through node v, ends[v] = number of words ending at v.
 * insert(s, -1) erases one copy of s (it must be present).
 * Walk nxt[][] directly for custom queries (e.g. greedy
 * lexicographic search, DP on the trie).
 * Usage: Trie t; t.insert("abc"); t.countWord("abc");
 * t.countPrefix("ab");
 * Time: O(|s|) per operation.
 * Status: stress-tested
 */
#pragma once

struct Trie {
	vector<array<int, 26>> nxt{{}}; vi pass{0}, ends{0};
	void insert(const string& s, int d = 1) {
		int v = 0;
		pass[0] += d;
		for (char ch : s) {
			int c = ch - 'a';
			if (!nxt[v][c]) {
				nxt[v][c] = sz(nxt);
				nxt.push_back({});
				pass.push_back(0), ends.push_back(0);
			}
			v = nxt[v][c], pass[v] += d;
		}
		ends[v] += d;
	}
	int find(const string& s) {  // node of s, or -1
		int v = 0;
		for (char ch : s) if (!(v = nxt[v][ch - 'a'])) return -1;
		return v;
	}
	int countWord(const string& s) {
		int v = find(s);
		return v < 0 ? 0 : ends[v];
	}
	int countPrefix(const string& s) {
		int v = find(s);
		return v < 0 ? 0 : pass[v];
	}
};
