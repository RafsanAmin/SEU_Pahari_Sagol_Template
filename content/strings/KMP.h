/**
 * Author: Team
 * License: CC0
 * Description: p[i] = length of the longest proper prefix of
 * s[0..i] that is also a suffix of it. match returns all start
 * positions of pat in s. Period of s: n - p[n-1] (a full period
 * if it divides n).
 * Usage: vi pos = match(s, pat); vi p = prefixFunction(s);
 * Time: O(N + M)
 * Status: stress-tested
 */
#pragma once

vi prefixFunction(const string& s) {
	vi p(sz(s));
	for (int i = 1; i < sz(s); i++) {
		int j = p[i - 1];
		while (j && s[i] != s[j]) j = p[j - 1];
		p[i] = j + (s[i] == s[j]);
	}
	return p;
}
vi match(const string& s, const string& pat) {
	vi p = prefixFunction(pat + '\0' + s), res;
	for (int i = sz(pat) + 1; i < sz(p); i++)
		if (p[i] == sz(pat)) res.push_back(i - 2 * sz(pat));
	return res;
}
