/**
 * Author: Team
 * License: CC0
 * Description: Linear basis of non-negative numbers under XOR.
 * insert returns false if x is already representable. The span
 * has 2\^{}rank elements. maxXor(x) = max of x XOR (any subset).
 * Usage: XorBasis b; b.insert(x); b.maxXor(); b.canMake(y);
 * Time: O(B) per operation.
 * Status: stress-tested
 */
#pragma once

struct XorBasis {
	static const int B = 63;
	ll b[B] = {}; int rank = 0;
	bool insert(ll x) {
		for (int i = B - 1; i >= 0; i--) if (x >> i & 1) {
			if (!b[i]) { b[i] = x, rank++; return true; }
			x ^= b[i];
		}
		return false;
	}
	ll maxXor(ll x = 0) {
		for (int i = B - 1; i >= 0; i--) x = max(x, x ^ b[i]);
		return x;
	}
	bool canMake(ll x) {
		for (int i = B - 1; i >= 0; i--)
			if (x >> i & 1 && b[i]) x ^= b[i];
		return x == 0;
	}
};
