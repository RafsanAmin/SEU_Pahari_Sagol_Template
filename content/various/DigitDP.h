/**
 * Author: Team
 * License: CC0
 * Description: Digit DP skeleton: count x in [0, N] by walking
 * the digits with a tight flag. Example: digit sum divisible by
 * K. Edit the state, transition and accept lines. Add a
 * "started" flag if leading zeros matter. Range [L, R]:
 * count(R) - count(L - 1).
 * Usage: ll c = countUpTo(N, K);
 * Time: O(digits \cdot 2 \cdot states \cdot 10)
 * Status: stress-tested
 */
#pragma once

ll countUpTo(ll N, int K) {
	if (N < 0) return 0;
	string D = to_string(N);
	int n = sz(D);
	auto memo = make_vec<ll>(n, 2, K, -1);
	auto go = [&](auto& f, int pos, bool tight, int st) -> ll {
		if (pos == n) return st == 0;  // EDIT: accept
		ll& r = memo[pos][tight][st];
		if (r != -1) return r;
		r = 0;
		int lim = tight ? D[pos] - '0' : 9;
		for (int d = 0; d <= lim; d++)  // EDIT: transition
			r += f(f, pos + 1, tight && d == lim, (st + d) % K);
		return r;
	};
	return go(go, 0, true, 0);
}
