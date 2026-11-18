#include "test.h"
#include "../content/data-structures/DSU.h"
#include "../content/data-structures/Fenwick.h"
#include "../content/data-structures/FenwickRange.h"
#include "../content/data-structures/Fenwick2D.h"
#include "../content/data-structures/SparseTable.h"
#include "../content/data-structures/Treap.h"
#include "../content/data-structures/MoQueries.h"
#include "../content/data-structures/LineContainer.h"
#include "../content/data-structures/LiChao.h"
#include "../content/data-structures/BinaryTrie.h"

void testDSU() {
    for (int it = 0; it < 500; it++) {
        int n = rint(1, 30);
        DSU d(n);
        vi lab(n);
        iota(all(lab), 0);
        for (int q = 0; q < 60; q++) {
            int a = rint(0, n - 1), b = rint(0, n - 1);
            if (rint(0, 1)) {
                bool merged = lab[a] != lab[b];
                CHECK(d.unite(a, b) == merged);
                int from = lab[b];
                for (int& x : lab) if (x == from) x = lab[a];
            } else {
                CHECK(d.same(a, b) == (lab[a] == lab[b]));
                CHECK(d.size(a) == count(all(lab), lab[a]));
            }
        }
    }
}

void testFenwick() {
    for (int it = 0; it < 500; it++) {
        int n = rint(1, 40);
        Fenwick f(n);
        FenwickRange fr(n);
        vll a(n), b(n);
        for (int q = 0; q < 100; q++) {
            int l = rint(0, n - 1), r = rint(l, n - 1), i = rint(0, n - 1);
            ll x = rint(0, 10), y = rint(-10, 10);
            int type = rint(0, 4);
            if (type == 0) a[i] += x, f.update(i, x);
            else if (type == 1) CHECK(f.query(l, r) == accumulate(a.begin() + l, a.begin() + r + 1, 0LL));
            else if (type == 2) {
                ll s = rint(-2, 120), pre = 0;
                int want = n;
                for (int j = 0; j < n; j++) if ((pre += a[j]) >= s) { want = j; break; }
                CHECK(f.lower_bound(s) == want);
            } else if (type == 3) {
                for (int j = l; j <= r; j++) b[j] += y;
                fr.update(l, r, y);
            } else CHECK(fr.query(l, r) == accumulate(b.begin() + l, b.begin() + r + 1, 0LL));
        }
    }
    for (int it = 0; it < 200; it++) {
        int n = rint(1, 8), m = rint(1, 8);
        Fenwick2D f(n, m);
        vector<vll> a(n, vll(m));
        for (int q = 0; q < 100; q++) {
            if (rint(0, 1)) {
                int x = rint(0, n - 1), y = rint(0, m - 1), v = rint(-5, 5);
                a[x][y] += v, f.update(x, y, v);
            } else {
                int x1 = rint(0, n - 1), x2 = rint(x1, n - 1), y1 = rint(0, m - 1), y2 = rint(y1, m - 1);
                ll s = 0;
                for (int i = x1; i <= x2; i++) for (int j = y1; j <= y2; j++) s += a[i][j];
                CHECK(f.query(x1, y1, x2, y2) == s);
            }
        }
    }
}

void testSparseTable() {
    for (int it = 0; it < 300; it++) {
        int n = rint(1, 50);
        vll a(n);
        for (ll& x : a) x = rint(-100, 100);
        SparseTable st(a);
        for (int l = 0; l < n; l++) for (int r = l; r < n; r++)
            CHECK(st.query(l, r) == *min_element(a.begin() + l, a.begin() + r + 1));
    }
}

void testTreap() {
    for (int it = 0; it < 300; it++) {
        Node* t = 0;
        vll a;
        for (int q = 0; q < 200; q++) {
            int type = rint(0, 4);
            if (type == 0 || a.empty()) {
                int pos = rint(0, sz(a));
                ll x = rint(-50, 50);
                t = insert(t, pos, new Node(x));
                a.insert(a.begin() + pos, x);
            } else if (type == 1) {
                int pos = rint(0, sz(a) - 1);
                t = erase(t, pos);
                a.erase(a.begin() + pos);
            } else if (type == 2) {
                int l = rint(0, sz(a) - 1), r = rint(l, sz(a) - 1);
                reverseRange(t, l, r);
                reverse(a.begin() + l, a.begin() + r + 1);
            } else {
                int l = rint(0, sz(a) - 1), r = rint(l, sz(a) - 1);
                CHECK(sumRange(t, l, r) == accumulate(a.begin() + l, a.begin() + r + 1, 0LL));
            }
            CHECK(cnt(t) == sz(a));
        }
        // full content check via split
        for (int i = 0; i < sz(a); i++) CHECK(sumRange(t, i, i) == a[i]);
    }
}

// Mo: number of distinct values in a[l..r]
vi moA, moCnt;
int moDistinct;
void add(int i) { if (moCnt[moA[i]]++ == 0) moDistinct++; }
void del(int i) { if (--moCnt[moA[i]] == 0) moDistinct--; }
ll calc() { return moDistinct; }
void testMo() {
    for (int it = 0; it < 200; it++) {
        int n = rint(1, 60);
        moA.assign(n, 0), moCnt.assign(10, 0), moDistinct = 0;
        for (int& x : moA) x = rint(0, 9);
        vector<pii> Q;
        for (int q = 0; q < 50; q++) {
            int l = rint(0, n - 1), r = rint(l, n - 1);
            Q.push_back({l, r});
        }
        vll res = mo(Q);
        for (int q = 0; q < sz(Q); q++) {
            set<int> s(moA.begin() + Q[q].first, moA.begin() + Q[q].second + 1);
            CHECK(res[q] == sz(s));
        }
    }
}

void testLines() {
    for (int it = 0; it < 300; it++) {
        LineContainer lc;
        int n = rint(1, 30);
        LiChao lic(n);
        vector<pll> lines;
        for (int q = 0; q < 60; q++) {
            if (lines.empty() || rint(0, 1)) {
                ll k = rint(-20, 20), m = rint(-100, 100);
                lines.push_back({k, m}), lc.add(k, m), lic.add(k, m);
            } else {
                ll x = rint(-50, 50);
                ll best = LLONG_MIN;
                for (auto [k, m] : lines) best = max(best, k * x + m);
                CHECK(lc.query(x) == best);
                int xi = rint(0, n - 1);
                ll mn = LLONG_MAX;
                for (auto [k, m] : lines) mn = min(mn, k * xi + m);
                CHECK(lic.query(xi) == mn);
            }
        }
    }
}

void testBinaryTrie() {
    for (int it = 0; it < 100; it++) {
        BinaryTrie t;
        multiset<int> s;
        for (int q = 0; q < 300; q++) {
            int x = rint(0, (1 << 12) - 1), type = rint(0, 2);
            if (type == 0 || s.empty()) t.insert(x), s.insert(x);
            else if (type == 1) {
                int y = *next(s.begin(), rint(0, sz(s) - 1));
                t.insert(y, -1), s.erase(s.find(y));
            } else {
                int best = 0;
                for (int y : s) best = max(best, x ^ y);
                CHECK(t.maxXor(x) == best);
            }
        }
    }
}

int main() {
    testDSU();
    testFenwick();
    testSparseTable();
    testTreap();
    testMo();
    testLines();
    testBinaryTrie();
    cout << "Data structures: OK" << endl;
}
