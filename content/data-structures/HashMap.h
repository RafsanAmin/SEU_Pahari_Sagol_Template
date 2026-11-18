/**
 * Author: Team (adapted from KACTL)
 * License: CC0
 * Description: Anti-hack hash for unordered\_map / gp\_hash\_table
 * (splitmix64 with a random seed). gp\_hash\_table is about
 * 3x faster than unordered\_map with the same API.
 * Usage: unordered_map<ll, int, chash> m;
 * gp_hash_table<ll, int, chash> h({}, {}, {}, {}, {1 << 16});
 * Time: O(1) expected.
 * Status: tested
 */
#pragma once

#include <ext/pb_ds/assoc_container.hpp> /** keep-include */
using namespace __gnu_pbds;
struct chash {
	static ull splitmix64(ull x) {
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}
	size_t operator()(ull x) const {
		static const ull R =
			chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + R);
	}
};
