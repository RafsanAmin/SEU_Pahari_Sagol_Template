/**
 * Author: Team (adapted from KACTL)
 * License: CC0
 * Description: Range update, range query. SegTree + a Tag.
 * Node() = identity, Tag() = no-op, compose(a, b): a becomes
 * "a then b". Example: range add, range sum and min.
 * Range assign: Tag \{ll v = 0; bool on = 0;\};
 * apply: if (t.on) a.sum = t.v * a.len, a.mn = t.v;
 * compose: if (b.on) a = b;
 * Usage: LazySegTree st(a); st.update(l, r, {x});
 * st.query(l, r).mn; // find_first: same as SegTree
 * Time: O(\log N) per operation, O(N) build.
 * Status: stress-tested
 */
#pragma once

struct LazySegTree
{
	// ---- EDIT HERE ----
	using T = ll;
	struct Node
	{
		ll sum = 0, mn = INF;
		int len = 0;
	};
	struct Tag
	{
		ll add = 0;
	};
	static Node leaf(T x) { return {x, x, 1}; }
	static Node merge(const Node &a, const Node &b)
	{
		return {a.sum + b.sum, min(a.mn, b.mn), a.len + b.len};
	}
	static void apply(Node &a, const Tag &t)
	{
		a.sum += t.add * a.len, a.mn += t.add;
	}
	static void compose(Tag &a, const Tag &b) { a.add += b.add; }
	// -------------------
	int n;
	vector<Node> t;
	vector<Tag> tag;
	LazySegTree(int n) : LazySegTree(vector<T>(n)) {}
	LazySegTree(const vector<T> &a)
			: n(sz(a)), t(4 * n), tag(4 * n)
	{
		build(a, 1, 0, n - 1);
	}
	void pull(int v) { t[v] = merge(t[2 * v], t[2 * v + 1]); }
	void put(int v, const Tag &g)
	{
		apply(t[v], g), compose(tag[v], g);
	}
	void push(int v)
	{
		put(2 * v, tag[v]), put(2 * v + 1, tag[v]), tag[v] = Tag();
	}
	void build(const vector<T> &a, int v, int l, int r)
	{
		if (l == r)
		{
			t[v] = leaf(a[l]);
			return;
		}
		int m = (l + r) / 2;
		build(a, 2 * v, l, m), build(a, 2 * v + 1, m + 1, r);
		pull(v);
	}
	void update(int ql, int qr, const Tag &g,
							int v, int l, int r)
	{
		if (qr < l || r < ql)
			return;
		if (ql <= l && r <= qr)
		{
			put(v, g);
			return;
		}
		push(v);
		int m = (l + r) / 2;
		update(ql, qr, g, 2 * v, l, m);
		update(ql, qr, g, 2 * v + 1, m + 1, r);
		pull(v);
	}
	Node query(int ql, int qr, int v, int l, int r)
	{
		if (qr < l || r < ql)
			return Node();
		if (ql <= l && r <= qr)
			return t[v];
		push(v);
		int m = (l + r) / 2;
		return merge(query(ql, qr, 2 * v, l, m),
								 query(ql, qr, 2 * v + 1, m + 1, r));
	}
	template <class F>
	int find_first(int ql, int qr, F &p,
								 Node &acc, int v, int l, int r)
	{
		if (qr < l || r < ql)
			return -1;
		if (ql <= l && r <= qr)
		{
			Node cur = merge(acc, t[v]);
			if (!p(cur))
			{
				acc = cur;
				return -1;
			}
			if (l == r)
				return l;
		}
		push(v);
		int m = (l + r) / 2;
		int res = find_first(ql, qr, p, acc, 2 * v, l, m);
		if (res != -1)
			return res;
		return find_first(ql, qr, p, acc, 2 * v + 1, m + 1, r);
	}
	void update(int l, int r, const Tag &g)
	{
		update(l, r, g, 1, 0, n - 1);
	}
	Node query(int l, int r) { return query(l, r, 1, 0, n - 1); }
	template <class F>
	int find_first(int l, int r, F p)
	{
		Node acc;
		return find_first(l, r, p, acc, 1, 0, n - 1);
	}
};
