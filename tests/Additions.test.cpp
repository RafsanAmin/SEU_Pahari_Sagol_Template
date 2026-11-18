// Tests for the beginner/intermediate additions and the self-written optional snippets.
#include "test.h"
#include "../content/strings/Trie.h"
#include "../content/strings/MinRotation.h"
#include "../content/strings/PalindromicTree.h"
#include "../content/data-structures/MergeSortTree.h"
#include "../content/data-structures/Monotonic.h"
#include "../content/data-structures/DynamicSegTree.h"
#include "../content/data-structures/WeightedDSU.h"
#include "../content/data-structures/Prefix2D.h"
#include "../content/data-structures/SegTreeBeats.h"
#include "../content/graph/BFS.h"
#include "../content/graph/Bipartite.h"
#include "../content/graph/FindCycle.h"
#include "../content/graph/EulerTour.h"
#include "../content/graph/TreeDiameter.h"
#include "../content/graph/VirtualTree.h"
#include "../content/number-theory/SegmentedSieve.h"
#include "../content/various/Inversions.h"
#include "../content/numerical/BigInt.h"

string rstr(int n, int alpha) {
    string s(n, 'a');
    for (char& c : s) c = 'a' + rint(0, alpha - 1);
    return s;
}

void testTrie() {
    for (int it = 0; it < 300; it++) {
        Trie t;
        multiset<string> ms;
        for (int q = 0; q < 100; q++) {
            string s = rstr(rint(0, 4), 2);
            int type = rint(0, 2);
            if (type == 0) t.insert(s), ms.insert(s);
            else if (type == 1 && ms.count(s)) t.insert(s, -1), ms.erase(ms.find(s));
            else {
                CHECK(t.countWord(s) == (int)ms.count(s));
                int pre = 0;
                for (auto& w : ms) pre += w.compare(0, sz(s), s) == 0;
                CHECK(t.countPrefix(s) == pre);
            }
        }
    }
}

void testStrings() {
    for (int it = 0; it < 2000; it++) {
        string s = rstr(rint(1, 12), rint(1, 3));
        int r = minRotation(s);
        string best = s;
        for (int i = 0; i < sz(s); i++) best = min(best, s.substr(i) + s.substr(0, i));
        CHECK(s.substr(r) + s.substr(0, r) == best);
        PalTree pt;
        for (char c : s) pt.add(c);
        pt.count();
        map<string, ll> occ;
        for (int i = 0; i < sz(s); i++) for (int j = i; j < sz(s); j++) {
            string u = s.substr(i, j - i + 1);
            if (u == string(u.rbegin(), u.rend())) occ[u]++;
        }
        CHECK(sz(pt.t) - 2 == sz(occ));
        // walk each node back to its string: rebuild via the parent of every nxt edge
        vector<string> str(sz(pt.t));
        vi done(sz(pt.t));
        done[0] = done[1] = 1, str[1] = "";
        for (int v = 0; v < sz(pt.t); v++) for (int c = 0; c < 26; c++) if (pt.t[v].nxt[c]) {
            int u = pt.t[v].nxt[c];
            string ch(1, char('a' + c));
            str[u] = pt.t[v].len == -1 ? ch : ch + str[v] + ch;
        }
        for (int v = 2; v < sz(pt.t); v++) CHECK(occ.count(str[v]) && occ[str[v]] == pt.t[v].cnt);
    }
}

void testDS() {
    for (int it = 0; it < 300; it++) {
        int n = rint(1, 30);
        vll a(n);
        for (ll& x : a) x = rint(-10, 10);
        MergeSortTree mt(a);
        for (int q = 0; q < 50; q++) {
            int l = rint(0, n - 1), r = rint(l, n - 1);
            ll x = rint(-12, 12);
            int c = 0;
            for (int i = l; i <= r; i++) c += a[i] <= x;
            CHECK(mt.query(l, r, x) == c);
            vll b(a.begin() + l, a.begin() + r + 1);
            sort(all(b));
            int k = rint(0, r - l);
            CHECK(mt.kth(l, r, k) == b[k]);
        }
        int k = rint(1, n);
        vll mins = slidingMin(a, k);
        CHECK(sz(mins) == n - k + 1);
        for (int i = 0; i + k <= n; i++) CHECK(mins[i] == *min_element(a.begin() + i, a.begin() + i + k));
        vi ng = nextGreater(a);
        for (int i = 0; i < n; i++) {
            int j = i + 1;
            while (j < n && a[j] <= a[i]) j++;
            CHECK(ng[i] == j);
        }
        vll orig = a, srt = a;
        ll inv = 0;
        for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) inv += a[i] > a[j];
        CHECK(inversions(srt) == inv && is_sorted(all(srt)));
    }
    // dynamic segtree on a huge range vs map
    for (int it = 0; it < 200; it++) {
        DynamicSegTree st(-1000000000000LL, 1000000000000LL);
        map<ll, ll> m;
        vll keys;
        for (int i = 0; i < 10; i++) keys.push_back((ll)rint(-1000000, 1000000) * 1000003);
        for (int q = 0; q < 100; q++) {
            ll i = keys[rint(0, 9)];
            if (rint(0, 1)) { ll x = rint(-50, 50); st.update(i, x), m[i] = x; }
            else {
                ll l = keys[rint(0, 9)], r = keys[rint(0, 9)];
                if (l > r) swap(l, r);
                ll s = 0;
                for (auto [k, v] : m) if (l <= k && k <= r) s += v;
                CHECK(st.query(l, r).sum == s);
            }
        }
    }
    // weighted DSU vs brute potentials
    for (int it = 0; it < 500; it++) {
        int n = rint(1, 10);
        WeightedDSU d(n);
        vll pot(n);
        for (ll& x : pot) x = rint(-20, 20);
        vi comp(n);
        iota(all(comp), 0);
        for (int q = 0; q < 40; q++) {
            int a = rint(0, n - 1), b = rint(0, n - 1);
            bool truth = rint(0, 3) != 0;
            ll w = truth ? pot[b] - pot[a] : rint(-40, 40);
            bool consistent = comp[a] != comp[b] ? truth : (pot[b] - pot[a] == w);
            // only add constraints that are true for the hidden potentials, or check rejection
            if (comp[a] != comp[b] && !truth) continue;
            CHECK(d.unite(a, b, w) == consistent);
            if (comp[a] != comp[b]) { int f = comp[b]; for (int& c : comp) if (c == f) c = comp[a]; }
            int x = rint(0, n - 1), y = rint(0, n - 1);
            CHECK(d.same(x, y) == (comp[x] == comp[y]));
            if (comp[x] == comp[y]) CHECK(d.diff(x, y) == pot[y] - pot[x]);
        }
    }
    for (int it = 0; it < 200; it++) {
        int n = rint(1, 7), m = rint(1, 7);
        vector<vll> a(n, vll(m));
        for (auto& row : a) for (ll& x : row) x = rint(-9, 9);
        Prefix2D p(a);
        int x1 = rint(0, n - 1), x2 = rint(x1, n - 1), y1 = rint(0, m - 1), y2 = rint(y1, m - 1);
        ll s = 0;
        for (int i = x1; i <= x2; i++) for (int j = y1; j <= y2; j++) s += a[i][j];
        CHECK(p.query(x1, y1, x2, y2) == s);
    }
    for (int it = 0; it < 300; it++) {
        int n = rint(1, 30);
        vll a(n);
        for (ll& x : a) x = rint(-20, 20);
        SegTreeBeats st(a);
        for (int q = 0; q < 100; q++) {
            int l = rint(0, n - 1), r = rint(l, n - 1);
            if (rint(0, 1)) {
                ll x = rint(-25, 25);
                for (int i = l; i <= r; i++) a[i] = min(a[i], x);
                st.chmin(l, r, x);
            } else {
                auto res = st.query(l, r);
                CHECK(res.sum == accumulate(a.begin() + l, a.begin() + r + 1, 0LL));
                CHECK(res.mx == *max_element(a.begin() + l, a.begin() + r + 1));
            }
        }
    }
}

void testGraphs() {
    for (int it = 0; it < 2000; it++) {
        int n = rint(1, 8), m = rint(0, 12);
        vector<vi> dg(n), ug(n);
        vector<vector<pii>> ugi(n), w01(n);
        vector<array<int, 3>> ue;
        for (int i = 0; i < m; i++) {
            int u = rint(0, n - 1), v = rint(0, n - 1), w = rint(0, 1);
            dg[u].push_back(v);
            ug[u].push_back(v), ug[v].push_back(u);
            ugi[u].push_back({v, i}), ugi[v].push_back({u, i});
            if (u == v) ugi[u].pop_back();  // self-loop listed once is enough
            w01[u].push_back({v, w});
            ue.push_back({u, v, w});
        }
        // directed cycle
        vi c = cycleDirected(dg);
        bool hasCyc;
        {
            vi indeg(n), q;
            for (auto& adj : dg) for (int v : adj) indeg[v]++;
            for (int i = 0; i < n; i++) if (!indeg[i]) q.push_back(i);
            for (int i = 0; i < sz(q); i++) for (int v : dg[q[i]]) if (!--indeg[v]) q.push_back(v);
            hasCyc = sz(q) < n;
        }
        CHECK(!c.empty() == hasCyc);
        for (int i = 0; i < sz(c); i++) {
            int a = c[i], b = c[(i + 1) % sz(c)];
            CHECK(find(all(dg[a]), b) != dg[a].end());
        }
        CHECK(set<int>(all(c)).size() == c.size());
        // undirected cycle: exists iff some edge is not needed for connectivity
        vi uc = cycleUndirected(ugi);
        int comps = n;
        {
            vi p(n); iota(all(p), 0);
            function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
            for (auto [u, v, w] : ue) if (f(u) != f(v)) p[f(u)] = f(v), comps--;
        }
        CHECK(!uc.empty() == (m > n - comps));
        CHECK(set<int>(all(uc)).size() == uc.size());
        if (sz(uc) >= 3) for (int i = 0; i < sz(uc); i++) {
            int a = uc[i], b = uc[(i + 1) % sz(uc)];
            CHECK(find(all(ug[a]), b) != ug[a].end());
        }
        // bipartite
        vi col = bipartite(ug);
        bool bip;
        {
            bool any = false;
            for (int mask = 0; mask < 1 << n && !any; mask++) {
                bool ok = true;
                for (auto [u, v, w] : ue) ok &= (mask >> u & 1) != (mask >> v & 1);
                any |= ok;
            }
            bip = any;
        }
        CHECK(!col.empty() == bip || n == 0);
        if (!col.empty()) for (auto [u, v, w] : ue) CHECK(col[u] != col[v]);
        // BFS (multi-source) and 0-1 BFS vs Floyd
        vector<vll> d(n, vll(n, INF)), d01 = d;
        for (int i = 0; i < n; i++) d[i][i] = d01[i][i] = 0;
        for (auto [u, v, w] : ue) {
            d[u][v] = d[v][u] = min(d[u][v], 1LL);
            d01[u][v] = min(d01[u][v], (ll)w);
        }
        for (int k = 0; k < n; k++) for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) {
            d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
            d01[i][j] = min(d01[i][j], d01[i][k] + d01[k][j]);
        }
        int s1 = rint(0, n - 1), s2 = rint(0, n - 1);
        vi bd = bfs(ug, {s1, s2});
        vll bd01 = bfs01(w01, s1);
        for (int v = 0; v < n; v++) {
            ll want = min(d[s1][v], d[s2][v]);
            CHECK(bd[v] == (want >= INF ? -1 : want));
            CHECK(bd01[v] == d01[s1][v]);
        }
    }
    // trees: Euler tour, diameter, virtual tree
    for (int it = 0; it < 300; it++) {
        int n = rint(1, 40), root = rint(0, n - 1);
        auto g = randomTree(n);
        EulerTour et(g, root);
        LCA L(g, root);
        vi dep = bfsDist(g, root);
        for (int a = 0; a < n; a++) {
            CHECK(et.order[et.tin[a]] == a && et.dep[a] == dep[a]);
            for (int b = 0; b < n; b++) CHECK(et.isAnc(a, b) == (L.lca(a, b) == a));
        }
        for (int a = 0; a < n; a++) {
            int cnt = 0;
            for (int b = 0; b < n; b++) cnt += L.lca(a, b) == a;
            CHECK(et.tout[a] - et.tin[a] + 1 == cnt);
        }
        vi path = diameter(g);
        int best = 0;
        for (int a = 0; a < n; a++) { vi d = bfsDist(g, a); best = max(best, *max_element(all(d))); }
        CHECK(sz(path) - 1 == best);
        for (int i = 0; i + 1 < sz(path); i++) CHECK(find(all(g[path[i]]), path[i + 1]) != g[path[i]].end());
        // virtual tree: parent of each listed node = nearest listed proper ancestor
        int k = rint(1, n);
        vi nodes(n);
        iota(all(nodes), 0);
        shuffle(all(nodes), test_rng);
        nodes.resize(k);
        auto vt = virtualTree(L, nodes);
        set<int> listed;
        for (auto [p, v] : vt) listed.insert(v);
        for (int v : nodes) CHECK(listed.count(v));
        CHECK(sz(vt) <= 2 * k - 1);
        for (int i = 1; i < sz(vt); i++) {
            int v = vt[i].second, p = vt[vt[i].first].second;
            int x = L.up[0][v], want = -1;
            while (true) { if (listed.count(x) && x != v) { want = x; break; } if (x == root) break; x = L.up[0][x]; }
            CHECK(p == want);
        }
        CHECK(L.lca(vt[0].second, vt.back().second) == vt[0].second);
    }
}

void testNumbers() {
    for (int it = 0; it < 200; it++) {
        ll L = rint(0, 2000), R = L + rint(0, 500);
        if (it % 4 == 0) L += 999999000000LL, R += 999999000000LL;
        auto isP = segmentedSieve(L, R);
        for (ll x = L; x <= R; x++) {
            bool p = x >= 2;
            for (ll d = 2; d * d <= x && p; d++) p = x % d != 0;
            CHECK(isP[x - L] == p);
        }
    }
    for (int it = 0; it < 3000; it++) {
        ll a = rint(0, 1 << 30) * (ll)rint(0, 1 << 30), b = rint(0, 1 << 30);
        BigInt A(a), Bb(b), A2(to_string(a));
        CHECK(A == A2);
        __int128 prod = (__int128)a * b;
        stringstream ss, ss2;
        ss << A * Bb;
        string want;
        for (__int128 x = prod; x > 0; x /= 10) want += char('0' + int(x % 10));
        if (want.empty()) want = "0";
        reverse(all(want));
        CHECK(ss.str() == want);
        ss2 << A + Bb;
        CHECK(ss2.str() == to_string(a + b));
        if (a >= b) { stringstream s3; s3 << A - Bb; CHECK(s3.str() == to_string(a - b)); }
        ll m = rint(1, 1000000000);
        auto [q, r] = (A * Bb).divmod(m);
        CHECK(r == (ll)(prod % m));
        stringstream s4; s4 << q;
        CHECK(BigInt(s4.str()) == q);
        CHECK((A < Bb) == (a < b));
    }
    BigInt big("123456789012345678901234567890"), one(1);
    stringstream s; s << big * big + one;
    CHECK(s.str() == "15241578753238836750495351562536198787501905199875019052101");
}

int main() {
    testTrie();
    testStrings();
    testDS();
    testGraphs();
    testNumbers();
    cout << "Additions: OK" << endl;
}
