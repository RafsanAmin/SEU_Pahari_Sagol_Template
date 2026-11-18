/**
 * Author: Team (adapted from KACTL)
 * License: CC0
 * Description: Offline range queries [l, r] by moving two
 * pointers. Write add/del/calc for the problem.
 * blk should be about N / sqrt(Q).
 * Usage: // write add / del / calc above mo(), then:
 * vector<pii> q = {{l, r}, ...}; vll ans = mo(q);
 * Time: O((N + Q) \sqrt Q) calls to add/del.
 * Status: stress-tested
 */
#pragma once

void add(int i);  // EDIT: add a[i] to the window
void del(int i);  // EDIT: remove a[i] from the window
ll calc();        // EDIT: answer for the current window

vll mo(const vector<pii>& Q) {
	int L = 0, R = -1, blk = 350;
	vi s(sz(Q)); vll res(sz(Q));
	iota(all(s), 0);
	sort(all(s), [&](int a, int b) {
		int x = Q[a].first / blk, y = Q[b].first / blk;
		if (x != y) return x < y;
		return x & 1 ? Q[a].second > Q[b].second
		             : Q[a].second < Q[b].second;
	});
	for (int qi : s) {
		auto [l, r] = Q[qi];
		while (R < r) add(++R);
		while (L > l) add(--L);
		while (R > r) del(R--);
		while (L < l) del(L++);
		res[qi] = calc();
	}
	return res;
}
