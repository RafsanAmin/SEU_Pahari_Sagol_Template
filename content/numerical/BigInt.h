/**
 * Author: Team
 * License: CC0
 * Description: Non-negative big integers in base 1e9: +, - (needs
 * a >= b), *, division / mod by a small number, comparison,
 * string I/O. Multiplication is schoolbook O(nm).
 * Usage: BigInt a("123456789012345678901"), b(42);
 * cout << a * b + a; auto [q, r] = a.divmod(7);
 * Time: + and - O(N), * O(NM), divmod O(N)
 * Status: stress-tested
 */
#pragma once

struct BigInt {
	static const int B = 1e9;
	vi d;  // little-endian base-B digits, no leading zeros
	BigInt(ll x = 0) { for (; x; x /= B) d.push_back(x % B); }
	BigInt(const string& s) {
		for (int i = sz(s); i > 0; i -= 9) {
			int j = max(0, i - 9);
			d.push_back(stoi(s.substr(j, i - j)));
		}
		trim();
	}
	void trim() { while (!d.empty() && !d.back()) d.pop_back(); }
	friend BigInt operator+(BigInt a, const BigInt& b) {
		ll c = 0;
		for (int i = 0; i < max(sz(a.d), sz(b.d)) || c; i++) {
			if (i == sz(a.d)) a.d.push_back(0);
			c += a.d[i] + (i < sz(b.d) ? b.d[i] : 0);
			a.d[i] = c % B, c /= B;
		}
		return a;
	}
	friend BigInt operator-(BigInt a, const BigInt& b) {
		ll c = 0;
		for (int i = 0; i < sz(b.d) || c; i++) {
			c = a.d[i] - c - (i < sz(b.d) ? b.d[i] : 0);
			a.d[i] = (c + B) % B, c = c < 0;
		}
		a.trim();
		return a;
	}
	friend BigInt operator*(const BigInt& a, const BigInt& b) {
		vector<ull> r(sz(a.d) + sz(b.d) + 1);
		for (int i = 0; i < sz(a.d); i++) {
			ull c = 0;
			for (int j = 0; j < sz(b.d) || c; j++) {
				c += r[i+j];
				if (j < sz(b.d)) c += 1ULL * a.d[i] * b.d[j];
				r[i + j] = c % B, c /= B;
			}
		}
		BigInt res;
		res.d.assign(all(r));
		res.trim();
		return res;
	}
	pair<BigInt, ll> divmod(ll m) const {  // m < 9e9
		BigInt q = *this; ll r = 0;
		for (int i = sz(q.d) - 1; i >= 0; i--) {
			r = r * B + q.d[i];
			q.d[i] = int(r / m), r %= m;
		}
		q.trim();
		return {q, r};
	}
	bool operator<(const BigInt& o) const {
		if (sz(d) != sz(o.d)) return sz(d) < sz(o.d);
		return lexicographical_compare(d.rbegin(), d.rend(),
			o.d.rbegin(), o.d.rend());
	}
	bool operator==(const BigInt& o) const { return d == o.d; }
	friend ostream& operator<<(ostream& os, const BigInt& a) {
		if (a.d.empty()) return os << 0;
		os << a.d.back();
		for (int i = sz(a.d) - 2; i >= 0; i--)
			os << setw(9) << setfill('0') << a.d[i];
		return os << setfill(' ');
	}
};
