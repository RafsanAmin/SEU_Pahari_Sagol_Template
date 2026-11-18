#include "test.h"
#include "../content/data-structures/SegTree.h"
#include "../content/data-structures/LazySegTree.h"
#include "../content/data-structures/PersistentSegTree.h"

void testSegTree() {
    for (int it = 0; it < 2000; it++) {
        int n = rint(1, 40);
        vll a(n);
        for (ll& x : a) x = rint(0, 20);
        SegTree st = it % 2 ? SegTree(a) : SegTree(n);
        if (it % 2 == 0) for (int i = 0; i < n; i++) st.update(i, a[i]);
        for (int q = 0; q < 100; q++) {
            int l = rint(0, n - 1), r = rint(l, n - 1), type = rint(0, 2);
            if (type == 0) {
                int i = rint(0, n - 1);
                a[i] = rint(0, 20);
                st.update(i, a[i]);
            } else if (type == 1) {
                CHECK(st.query(l, r).sum == accumulate(a.begin() + l, a.begin() + r + 1, 0LL));
            } else {
                ll k = rint(0, 150), s = 0;
                int want = -1;
                for (int i = l; i <= r; i++) if ((s += a[i]) >= k) { want = i; break; }
                CHECK(st.find_first(l, r, [&](const SegTree::Node& v) { return v.sum >= k; }) == want);
            }
        }
    }
}

void testLazy() {
    for (int it = 0; it < 2000; it++) {
        int n = rint(1, 40);
        vll a(n);
        for (ll& x : a) x = rint(-20, 20);
        LazySegTree st = it % 2 ? LazySegTree(a) : LazySegTree(n);
        if (it % 2 == 0) fill_all(a, 0);
        for (int q = 0; q < 100; q++) {
            int l = rint(0, n - 1), r = rint(l, n - 1), type = rint(0, 2);
            if (type == 0) {
                ll x = rint(-10, 10);
                for (int i = l; i <= r; i++) a[i] += x;
                st.update(l, r, {x});
            } else if (type == 1) {
                auto res = st.query(l, r);
                CHECK(res.sum == accumulate(a.begin() + l, a.begin() + r + 1, 0LL));
                CHECK(res.mn == *min_element(a.begin() + l, a.begin() + r + 1));
                CHECK(res.len == r - l + 1);
            } else {
                ll x = rint(-30, 30);
                int want = -1;
                for (int i = l; i <= r; i++) if (a[i] <= x) { want = i; break; }
                CHECK(st.find_first(l, r, [&](const LazySegTree::Node& v) { return v.mn <= x; }) == want);
            }
        }
    }
}

void testPersistent() {
    for (int it = 0; it < 1000; it++) {
        int n = rint(1, 30);
        PersistentSegTree pst(n);
        vector<vll> ver;
        vi roots;
        ver.push_back(vll(n));
        for (ll& x : ver[0]) x = rint(-20, 20);
        roots.push_back(pst.build(ver[0]));
        for (int q = 0; q < 100; q++) {
            int v = rint(0, sz(roots) - 1), l = rint(0, n - 1), r = rint(l, n - 1);
            if (rint(0, 1)) {
                int i = rint(0, n - 1);
                ll x = rint(-20, 20);
                ver.push_back(ver[v]);
                ver.back()[i] = x;
                roots.push_back(pst.update(roots[v], i, x));
            } else {
                CHECK(pst.query(roots[v], l, r).sum ==
                      accumulate(ver[v].begin() + l, ver[v].begin() + r + 1, 0LL));
            }
        }
    }
    // k-th smallest in a range, exactly as in the Usage line
    for (int it = 0; it < 500; it++) {
        int n = rint(1, 30), m = rint(1, 10);
        vi a(n);
        for (int& x : a) x = rint(0, m - 1);
        PersistentSegTree pst(m);
        vi root(n + 1, 0);
        for (int i = 0; i < n; i++)
            root[i + 1] = pst.update(root[i], a[i], pst.query(root[i], a[i], a[i]).sum + 1);
        for (int q = 0; q < 50; q++) {
            int l = rint(0, n - 1), r = rint(l, n - 1), k = rint(0, r - l);
            vi b(a.begin() + l, a.begin() + r + 1);
            sort(all(b));
            CHECK(pst.kth(root[r + 1], root[l], k) == b[k]);
        }
    }
}

int main() {
    testSegTree();
    testLazy();
    testPersistent();
    cout << "SegTree family: OK" << endl;
}
