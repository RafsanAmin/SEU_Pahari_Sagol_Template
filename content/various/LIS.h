/**
 * Author: Johan Sannemo (KACTL)
 * License: CC0
 * Description: Indices of a longest strictly increasing
 * subsequence. For non-decreasing, change \{S[i], 0\} to
 * \{S[i], i\}. Length only: sz(lis(S)).
 * Usage: vi idx = lis(a);
 * Time: O(N \log N)
 * Status: stress-tested
 */
#pragma once

template<class I> vi lis(const vector<I>& S) {
	if (S.empty()) return {};
	vi prev(sz(S));
	typedef pair<I, int> p;
	vector<p> res;
	for (int i = 0; i < sz(S); i++) {
		auto it = lower_bound(all(res), p{S[i], 0});
		if (it == res.end())
			res.emplace_back(), it = res.end() - 1;
		*it = {S[i], i};
		prev[i] = it == res.begin() ? 0 : (it - 1)->second;
	}
	int L = sz(res), cur = res.back().second;
	vi ans(L);
	while (L--) ans[L] = cur, cur = prev[cur];
	return ans;
}
