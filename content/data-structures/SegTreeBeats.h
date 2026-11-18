/**
 * Author: Team
 * License: CC0
 * Description: Segment tree beats (Ji). chmin(l, r, x) sets
 * a[i] = min(a[i], x) on [l, r]; query gives sum and max.
 * Each node keeps max, strict second max and count of max.
 * Usage: SegTreeBeats st(a); st.chmin(l, r, x);
 * st.query(l, r).sum;
 * Time: O((N + Q) \log N) amortized.
 * Status: stress-tested
 */
#pragma once

struct SegTreeBeats {
	struct Node { ll mx = -INF, smx = -INF, cnt = 0, sum = 0; };
	int n; vector<Node> t;
	SegTreeBeats(const vll& a) : n(sz(a)), t(4 * n) {
		build(a, 1, 0, n - 1);
	}
	static Node merge(const Node& a, const Node& b) {
		Node c;
		c.sum = a.sum + b.sum, c.mx = max(a.mx, b.mx);
		c.cnt = (a.mx == c.mx ? a.cnt : 0)
		      + (b.mx == c.mx ? b.cnt : 0);
		c.smx = max(a.mx == c.mx ? a.smx : a.mx,
		            b.mx == c.mx ? b.smx : b.mx);
		return c;
	}
	void pull(int v) { t[v] = merge(t[2*v], t[2*v+1]); }
	void put(int v, ll x) {  // needs smx < x
		if (x >= t[v].mx) return;
		t[v].sum -= (t[v].mx - x) * t[v].cnt, t[v].mx = x;
	}
	void push(int v) { put(2*v, t[v].mx), put(2*v+1, t[v].mx); }
	void build(const vll& a, int v, int l, int r) {
		if (l == r) { t[v] = {a[l], -INF, 1, a[l]}; return; }
		int m = (l + r) / 2;
		build(a, 2*v, l, m), build(a, 2*v+1, m+1, r);
		pull(v);
	}
	void chmin(int ql, int qr, ll x, int v, int l, int r) {
		if (qr < l || r < ql || t[v].mx <= x) return;
		if (ql <= l && r <= qr && t[v].smx < x) {
			put(v, x);
			return;
		}
		push(v);
		int m = (l + r) / 2;
		chmin(ql, qr, x, 2*v, l, m);
		chmin(ql, qr, x, 2*v+1, m+1, r);
		pull(v);
	}
	Node query(int ql, int qr, int v, int l, int r) {
		if (qr < l || r < ql) return Node();
		if (ql <= l && r <= qr) return t[v];
		push(v);
		int m = (l + r) / 2;
		return merge(query(ql, qr, 2*v, l, m),
		             query(ql, qr, 2*v+1, m+1, r));
	}
	void chmin(int l, int r, ll x) { chmin(l, r, x, 1, 0, n-1); }
	Node query(int l, int r) { return query(l, r, 1, 0, n - 1); }
};
