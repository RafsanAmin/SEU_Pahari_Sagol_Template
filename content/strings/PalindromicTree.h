/**
 * Author: Team
 * License: CC0
 * Description: Palindromic tree (eertree) over 'a'..'z': one
 * node per distinct palindromic substring (nodes 0 and 1 are
 * the roots of length -1 and 0). cnt[v] after calling count()
 * = number of occurrences of node v's palindrome.
 * Usage: PalTree pt; for (char c : s) pt.add(c);
 * sz(pt.t) - 2;  // distinct palindromes
 * Time: O(N)
 * Status: stress-tested
 */
#pragma once

struct PalTree {
	struct St { int len, link; array<int, 26> nxt; ll cnt; };
	vector<St> t{{-1, 0, {}, 0}, {0, 0, {}, 0}};
	string s; int last = 1;
	int jump(int v, int i) {
		while (i - 1 - t[v].len < 0 || s[i - 1 - t[v].len] != s[i])
			v = t[v].link;
		return v;
	}
	void add(char ch) {
		s += ch;
		int i = sz(s) - 1, c = ch - 'a', v = jump(last, i);
		if (!t[v].nxt[c]) {
			int lnk = t[v].len < 0 ? 1
			        : t[jump(t[v].link, i)].nxt[c];
			t.push_back({t[v].len + 2, lnk, {}, 0});
			t[v].nxt[c] = sz(t) - 1;
		}
		last = t[v].nxt[c], t[last].cnt++;
	}
	void count() {  // call once, after all add()s
		for (int v = sz(t) - 1; v > 1; v--)
			t[t[v].link].cnt += t[v].cnt;
	}
};
