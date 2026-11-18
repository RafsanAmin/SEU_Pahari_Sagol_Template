/**
 * Author: Team
 * License: CC0
 * Description: Arithmetic modulo a prime MOD. Division uses
 * Fermat, so MOD must be prime (and b != 0).
 * Usage: Mint a = 5, b; cin >> b; a += b; a *= 3;
 * Mint c = a.pow(10) / b; c.inv(); cout << c.v;
 * Time: O(1), pow/inv/division O(\log MOD).
 * Status: stress-tested
 */
#pragma once

const int MOD = 998244353;  // or 1e9 + 7
struct Mint {
	int v;
	Mint(ll x = 0) : v(int(x % MOD)) { if (v < 0) v += MOD; }
	Mint& operator+=(Mint o) {
		if ((v += o.v) >= MOD) v -= MOD;
		return *this;
	}
	Mint& operator-=(Mint o) {
		if ((v -= o.v) < 0) v += MOD;
		return *this;
	}
	Mint& operator*=(Mint o) {
		v = int(1LL * v * o.v % MOD);
		return *this;
	}
	Mint& operator/=(Mint o) { return *this *= o.inv(); }
	friend Mint operator+(Mint a, Mint b) { return a += b; }
	friend Mint operator-(Mint a, Mint b) { return a -= b; }
	friend Mint operator*(Mint a, Mint b) { return a *= b; }
	friend Mint operator/(Mint a, Mint b) { return a /= b; }
	Mint operator-() const { return Mint() - *this; }
	bool operator==(Mint o) const { return v == o.v; }
	bool operator!=(Mint o) const { return v != o.v; }
	Mint pow(ll e) const {
		Mint r = 1, b = *this;
		for (; e; e >>= 1, b *= b) if (e & 1) r *= b;
		return r;
	}
	Mint inv() const { return pow(MOD - 2); }
	friend ostream& operator<<(ostream& os, Mint a) {
		return os << a.v;
	}
	friend istream& operator>>(istream& is, Mint& a) {
		ll x; is >> x; a = Mint(x); return is;
	}
};
