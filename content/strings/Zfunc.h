/**
 * Author: Team
 * License: CC0
 * Description: z[i] = length of the longest common prefix of s
 * and s[i..]; z[0] = 0. Pattern search: zFunction(pat + '\#' + s).
 * Usage: vi z = zFunction(s);
 * Time: O(N)
 * Status: stress-tested
 */
#pragma once

vi zFunction(const string& s) {
	int n = sz(s);
	vi z(n);
	for (int i = 1, l = 0, r = 0; i < n; i++) {
		if (i < r) z[i] = min(r - i, z[i - l]);
		while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
		if (i + z[i] > r) l = i, r = i + z[i];
	}
	return z;
}
