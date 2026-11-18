/**
 * Author: Team (adapted from KACTL)
 * License: CC0
 * Description: Implicit treap: an array with O(log N) split,
 * merge, insert, erase, range reverse and range sum.
 * split(t, k) = (first k elements, rest). Edit Node/pull/push
 * to add other range data or lazy tags.
 * Usage: Node* t = 0; t = insert(t, pos, new Node(x));
 * t = erase(t, pos); sumRange(t, l, r); reverseRange(t, l, r);
 * auto [a, b] = split(t, k); t = merge(a, b);
 * Time: O(\log N) expected per operation.
 * Status: stress-tested
 */
#pragma once

mt19937 trng(chrono::steady_clock::now()
	.time_since_epoch().count());
struct Node {
	Node *l = 0, *r = 0;
	int pri = (int)trng(), cnt = 1;
	ll val, sum; bool rev = 0;  // EDIT: data + lazy tags
	Node(ll v) : val(v), sum(v) {}
};
int cnt(Node* t) { return t ? t->cnt : 0; }
ll sum(Node* t) { return t ? t->sum : 0; }
void pull(Node* t) {
	t->cnt = cnt(t->l) + cnt(t->r) + 1;
	t->sum = sum(t->l) + sum(t->r) + t->val;
}
void push(Node* t) {
	if (!t || !t->rev) return;
	swap(t->l, t->r), t->rev = 0;
	if (t->l) t->l->rev ^= 1;
	if (t->r) t->r->rev ^= 1;
}
pair<Node*, Node*> split(Node* t, int k) {
	if (!t) return {};
	push(t);
	if (cnt(t->l) >= k) {
		auto [a, b] = split(t->l, k);
		t->l = b, pull(t);
		return {a, t};
	}
	auto [a, b] = split(t->r, k - cnt(t->l) - 1);
	t->r = a, pull(t);
	return {t, b};
}
Node* merge(Node* a, Node* b) {
	if (!a || !b) return a ? a : b;
	if (a->pri > b->pri) {
		push(a), a->r = merge(a->r, b), pull(a);
		return a;
	}
	push(b), b->l = merge(a, b->l), pull(b);
	return b;
}
Node* insert(Node* t, int pos, Node* x) {  // x at index pos
	auto [a, b] = split(t, pos);
	return merge(merge(a, x), b);
}
Node* erase(Node* t, int pos) {
	auto [a, b] = split(t, pos);
	return merge(a, split(b, 1).second);
}
void reverseRange(Node*& t, int l, int r) {
	auto [a, b] = split(t, l);
	auto [c, d] = split(b, r - l + 1);
	c->rev ^= 1;
	t = merge(a, merge(c, d));
}
ll sumRange(Node*& t, int l, int r) {
	auto [a, b] = split(t, l);
	auto [c, d] = split(b, r - l + 1);
	ll res = sum(c);
	t = merge(a, merge(c, d));
	return res;
}
