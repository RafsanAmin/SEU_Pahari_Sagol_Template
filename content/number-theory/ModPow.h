/**
 * Author: Noam527
 * Date: 2019-04-24
 * License: CC0
 * Source: folklore
 * Description: b\^{}e modulo the global prime mod (KACTL helper,
 * used by MatrixInverse-mod).
 * Usage: ll x = modpow(b, e);  // b^e modulo the global const mod
 * Time: O(\log e)
 * Status: tested
 */
#pragma once

const ll mod = 1000000007; // faster if const

ll modpow(ll b, ll e) {
	ll ans = 1;
	for (; e; b = b * b % mod, e /= 2)
		if (e & 1) ans = ans * b % mod;
	return ans;
}
