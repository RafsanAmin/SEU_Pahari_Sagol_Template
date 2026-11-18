/**
 * Author: Team
 * License: CC0
 * Description: Suffix automaton: the minimal DFA of all
 * substrings. Each state is a set of substrings ending at the
 * same positions, lengths in (len[link], len]. Occurrences of
 * state v: set occ = 1 for non-clone states, then add occ[v] to
 * occ[link[v]] in decreasing len order.
 * Usage: SuffixAutomaton sa(s); sa.contains(t); sa.distinct();
 * Time: O(N \log \Sigma)
 * Status: stress-tested
 */
#pragma once

struct SuffixAutomaton {
	struct St { int len, link; map<char, int> next; };
	vector<St> st{{0, -1, {}}}; int last = 0;
	SuffixAutomaton(const string& s) {
		for (char c : s) extend(c);
	}
	void extend(char c) {
		int cur = sz(st), p = last;
		st.push_back({st[last].len + 1, 0, {}});
		while (p != -1 && !st[p].next.count(c))
			st[p].next[c] = cur, p = st[p].link;
		if (p != -1) {
			int q = st[p].next[c];
			if (st[p].len + 1 == st[q].len) st[cur].link = q;
			else {
				int cl = sz(st);
				st.push_back({st[p].len + 1, st[q].link, st[q].next});
				while (p != -1 && st[p].next[c] == q)
					st[p].next[c] = cl, p = st[p].link;
				st[q].link = st[cur].link = cl;
			}
		}
		last = cur;
	}
	bool contains(const string& t) {
		int v = 0;
		for (char c : t) {
			auto it = st[v].next.find(c);
			if (it == st[v].next.end()) return false;
			v = it->second;
		}
		return true;
	}
	ll distinct() {  // number of distinct non-empty substrings
		ll r = 0;
		for (int i = 1; i < sz(st); i++)
			r += st[i].len - st[st[i].link].len;
		return r;
	}
};
