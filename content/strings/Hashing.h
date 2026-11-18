/**
 * Author: Team
 * License: CC0
 * Description: Polynomial hashing mod $2^{61}-1$ with a random
 * base (safe against anti-hash tests). get(l, r) = hash of
 * s[l..r]. All Hash objects share the base, so substrings of
 * different strings can be compared.
 * Usage: Hash h(s); h.get(l1, r1) == h.get(l2, r2);
 * Time: O(N) build, O(1) per query.
 * Status: stress-tested
 */
#pragma once

struct Hash {
	static const ull M = (1ULL << 61) - 1;
	static ull mul(ull a, ull b) {
		unsigned __int128 c = (unsigned __int128)a * b;
		ull r = ull(c & M) + ull(c >> 61);
		return r >= M ? r - M : r;
	}
	inline static const ull B = mt19937_64(
		chrono::steady_clock::now().time_since_epoch().count())()
		% (M - 1000) + 500;
	vector<ull> h, pw;
	Hash(const string& s) : h(sz(s) + 1), pw(sz(s) + 1, 1) {
		for (int i = 0; i < sz(s); i++) {
			h[i + 1] = (mul(h[i], B) + (unsigned char)s[i]) % M;
			pw[i + 1] = mul(pw[i], B);
		}
	}
	ull get(int l, int r) {
		return (h[r + 1] + M - mul(h[l], pw[r - l + 1])) % M;
	}
};
