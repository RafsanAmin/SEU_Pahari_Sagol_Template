/**
 * Author: Team
 * License: CC0
 * Description: d1[i] = number of odd palindromes centred at i
 * (longest has length 2 d1[i] - 1). d2[i] = number of even
 * palindromes centred between i-1 and i (longest: 2 d2[i]).
 * s[l..r] is a palindrome iff, with len = r - l + 1:
 * odd len: d1[(l+r)/2] >= (len+1)/2, even: d2[(l+r+1)/2] >= len/2.
 * Usage: auto [d1, d2] = manacher(s);
 * Time: O(N)
 * Status: stress-tested
 */
#pragma once

array<vi, 2> manacher(const string& s) {
	int n = sz(s);
	vi d1(n), d2(n);
	for (int i = 0, l = 0, r = -1; i < n; i++) {
		int k = i > r ? 1 : min(d1[l + r - i], r - i + 1);
		while (k <= i && i + k < n && s[i-k] == s[i+k]) k++;
		d1[i] = k--;
		if (i + k > r) l = i - k, r = i + k;
	}
	for (int i = 0, l = 0, r = -1; i < n; i++) {
		int k = i > r ? 0 : min(d2[l + r - i + 1], r - i + 1);
		while (k < i && i + k < n && s[i-k-1] == s[i+k]) k++;
		d2[i] = k--;
		if (i + k > r) l = i - k - 1, r = i + k;
	}
	return {d1, d2};
}
