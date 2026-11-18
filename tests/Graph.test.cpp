#include "test.h"
#include "../content/graph/Dijkstra.h"
#include "../content/graph/BellmanFord.h"
#include "../content/graph/FloydWarshall.h"
#include "../content/graph/TopoSort.h"
#include "../content/graph/SCC.h"
#include "../content/graph/TwoSat.h"
#include "../content/graph/BridgesCuts.h"
#include "../content/graph/Kruskal.h"
#include "../content/graph/EulerWalk.h"
#include "../content/graph/Dinic.h"
#include "../content/graph/MCMF.h"
#include "../content/graph/HopcroftKarp.h"

// reach[u][v]: v reachable from u (directed)
vector<vector<bool>> reachability(int n, const vector<vi>& g) {
    vector<vector<bool>> r(n, vector<bool>(n));
    for (int s = 0; s < n; s++) { vi d = bfsDist(g, s); for (int v = 0; v < n; v++) r[s][v] = d[v] >= 0; }
    return r;
}

// independent shortest path brute: Bellman-Ford rounds + reachability for -INF
vll bruteSP(int n, const vector<array<ll, 3>>& e, int s) {
    vll d(n, INF); d[s] = 0;
    for (int it = 0; it < n - 1; it++)
        for (auto [u, v, w] : e) if (d[u] < INF) d[v] = min(d[v], d[u] + w);
    vector<vi> g(n);
    for (auto [u, v, w] : e) g[u].push_back(v);
    auto R = reachability(n, g);
    vll res = d;
    for (auto [u, v, w] : e) if (d[u] < INF && d[u] + w < d[v])
        for (int x = 0; x < n; x++) if (R[v][x]) res[x] = -INF;
    return res;
}

void testShortestPaths() {
    for (int it = 0; it < 500; it++) {
        int n = rint(1, 8), m = rint(0, 20);
        bool neg = it % 2;
        vector<array<ll, 3>> e;
        vector<vector<pair<int, ll>>> g(n);
        vector<vll> fw(n, vll(n, INF));
        for (int i = 0; i < n; i++) fw[i][i] = 0;
        for (int i = 0; i < m; i++) {
            int u = rint(0, n - 1), v = rint(0, n - 1);
            ll w = neg ? rint(-3, 10) : rint(0, 10);
            e.push_back({u, v, w}), g[u].push_back({v, w});
            fw[u][v] = min(fw[u][v], w);
        }
        floydWarshall(fw);
        for (int s = 0; s < n; s++) {
            vll want = bruteSP(n, e, s);
            CHECK(bellmanFord(n, e, s) == want);
            for (int v = 0; v < n; v++) CHECK(fw[s][v] == want[v]);
            if (!neg) {
                vi par;
                vll d = dijkstra(g, s, par);
                CHECK(d == want);
                for (int v = 0; v < n; v++) if (v != s && d[v] < INF) {
                    // walk parents back and check the path length
                    ll len = 0; int x = v;
                    while (x != s) {
                        int p = par[x]; ll best = INF;
                        for (auto [y, w] : g[p]) if (y == x) best = min(best, w);
                        len += best, x = p;
                    }
                    CHECK(len == d[v]);
                }
            }
        }
    }
}

void testTopoSCC() {
    for (int it = 0; it < 1000; it++) {
        int n = rint(1, 9), m = rint(0, 15);
        vector<vi> g(n);
        for (int i = 0; i < m; i++) g[rint(0, n - 1)].push_back(rint(0, n - 1));
        auto R = reachability(n, g);
        bool cyc = false;
        for (int u = 0; u < n; u++) for (int v : g[u]) if (R[v][u]) cyc = true;
        vi ord = topoSort(g);
        CHECK((sz(ord) == n) == !cyc);
        if (!cyc) {
            vi pos(n);
            for (int i = 0; i < n; i++) pos[ord[i]] = i;
            for (int u = 0; u < n; u++) for (int v : g[u]) CHECK(pos[u] < pos[v]);
        }
        SCC s(g);
        for (int u = 0; u < n; u++) for (int v = 0; v < n; v++)
            CHECK((s.comp[u] == s.comp[v]) == (R[u][v] && R[v][u]));
        for (int u = 0; u < n; u++) for (int v : g[u]) CHECK(s.comp[u] >= s.comp[v]);
        set<int> ids(all(s.comp));
        CHECK(sz(ids) == s.cnt && *ids.rbegin() == s.cnt - 1);
    }
}

void testTwoSat() {
    for (int it = 0; it < 2000; it++) {
        int n = rint(1, 6), m = rint(0, 12);
        vector<pii> cl;
        TwoSat ts(n);
        for (int i = 0; i < m; i++) {
            int a = rint(0, n - 1), b = rint(0, n - 1);
            if (rint(0, 1)) a = ~a;
            if (rint(0, 1)) b = ~b;
            cl.push_back({a, b}), ts.either(a, b);
        }
        auto lit = [&](int x, int mask) { return x >= 0 ? (mask >> x & 1) : !(mask >> ~x & 1); };
        bool any = false;
        for (int mask = 0; mask < 1 << n; mask++) {
            bool ok = true;
            for (auto [a, b] : cl) ok &= lit(a, mask) || lit(b, mask);
            any |= ok;
        }
        bool got = ts.solve();
        CHECK(got == any);
        if (got) {
            int mask = 0;
            for (int i = 0; i < n; i++) mask |= ts.val[i] << i;
            for (auto [a, b] : cl) CHECK(lit(a, mask) || lit(b, mask));
        }
    }
}

int components(int n, const vector<array<int, 3>>& e, int skipEdge, int skipNode) {
    DSU d(n);
    int c = n - (skipNode >= 0);
    for (auto [u, v, id] : e)
        if (id != skipEdge && u != skipNode && v != skipNode && d.unite(u, v)) c--;
    return c;
}

void testBridgesKruskal() {
    for (int it = 0; it < 1000; it++) {
        int n = rint(1, 8), m = rint(0, 12);
        vector<vector<pii>> g(n);
        vector<array<int, 3>> e;
        for (int i = 0; i < m; i++) {
            int u = rint(0, n - 1), v = rint(0, n - 1);
            if (u == v) continue;
            int id = sz(e);
            e.push_back({u, v, id}), g[u].push_back({v, id}), g[v].push_back({u, id});
        }
        LowLink L(g);
        int base = components(n, e, -1, -1);
        set<int> br(all(L.bridges));
        CHECK(sz(br) == sz(L.bridges));
        for (auto [u, v, id] : e) CHECK(br.count(id) == (components(n, e, id, -1) > base));
        for (int x = 0; x < n; x++) {
            int deg = 0;
            for (auto [u, v, id] : e) deg += u == x || v == x;
            // removing an isolated node lowers the count, never a cut
            bool cut = deg > 0 && components(n, e, -1, x) > base;
            CHECK(L.isCut[x] == cut);
        }
        // Kruskal vs all edge subsets
        vector<array<ll, 3>> we;
        for (auto [u, v, id] : e) we.push_back({rint(-5, 20), u, v});
        int k = sz(we);
        ll best = LLONG_MAX;
        for (int mask = 0; mask < 1 << k; mask++) {
            DSU d(n); bool ok = true; ll w = 0; int used = 0;
            for (int i = 0; i < k; i++) if (mask >> i & 1) {
                ok &= d.unite(we[i][1], we[i][2]), w += we[i][0], used++;
            }
            if (ok && used == n - base) best = min(best, w);
        }
        CHECK(kruskal(n, we) == best);
    }
}

void checkWalk(const vector<vector<pii>>& g, int m, int src, const vi& w, bool directed) {
    CHECK(sz(w) == m + 1 && w[0] == src);
    vi used(m);
    for (int i = 0; i + 1 < sz(w); i++) {
        bool found = false;
        for (auto [y, e] : g[w[i]]) if (y == w[i + 1] && !used[e]) { used[e] = 1, found = true; break; }
        CHECK(found);
    }
    (void)directed;
}

void testEuler() {
    for (int it = 0; it < 2000; it++) {
        int n = rint(1, 6), m = rint(1, 8);
        bool directed = it % 2;
        vector<vector<pii>> g(n);
        vi indeg(n), outdeg(n);
        vector<pii> edges;
        for (int i = 0; i < m; i++) {
            int u = rint(0, n - 1), v = rint(0, n - 1);
            edges.push_back({u, v});
            g[u].push_back({v, i});
            if (!directed && u != v) g[v].push_back({u, i});
            if (!directed && u == v) g[u].push_back({u, i});
            outdeg[u]++, indeg[v]++;
        }
        int src = rint(0, n - 1);
        vi w = eulerWalk(g, m, src);
        // brute: does an Euler walk from src exist? (DFS over edge orders)
        vi used(m);
        function<bool(int, int)> go = [&](int x, int cnt) -> bool {
            if (cnt == m) return true;
            for (auto [y, e] : g[x]) if (!used[e]) {
                used[e] = 1;
                if (go(y, cnt + 1)) { used[e] = 0; return true; }
                used[e] = 0;
            }
            return false;
        };
        bool exists = go(src, 0);
        CHECK(!w.empty() == exists);
        if (exists) checkWalk(g, m, src, w, directed);
    }
}

ll bruteMaxFlow(int n, vector<vll> cap, int s, int t) {
    ll flow = 0;
    while (true) {
        vi par(n, -1); par[s] = s;
        queue<int> q; q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v = 0; v < n; v++) if (par[v] < 0 && cap[u][v] > 0) par[v] = u, q.push(v);
        }
        if (par[t] < 0) return flow;
        ll f = INF;
        for (int v = t; v != s; v = par[v]) f = min(f, cap[par[v]][v]);
        for (int v = t; v != s; v = par[v]) cap[par[v]][v] -= f, cap[v][par[v]] += f;
        flow += f;
    }
}

void testFlows() {
    for (int it = 0; it < 1000; it++) {
        int n = rint(2, 7), m = rint(0, 14), s = 0, t = n - 1;
        vector<vll> cap(n, vll(n));
        Dinic d(n);
        vector<array<ll, 3>> es;
        for (int i = 0; i < m; i++) {
            int u = rint(0, n - 1), v = rint(0, n - 1);
            ll c = rint(0, 10);
            if (u == v) continue;
            cap[u][v] += c, d.addEdge(u, v, c), es.push_back({u, v, c});
        }
        ll want = bruteMaxFlow(n, cap, s, t);
        CHECK(d.calc(s, t) == want);
        ll cut = 0;
        for (auto [u, v, c] : es) if (d.leftOfMinCut(u) && !d.leftOfMinCut(v)) cut += c;
        CHECK(cut == want && d.leftOfMinCut(s) && !d.leftOfMinCut(t));
    }
    // MCMF: unit-capacity DAG (negative costs allowed), brute over edge subsets
    for (int it = 0; it < 1000; it++) {
        int n = rint(2, 6), m = rint(0, 10), s = 0, t = n - 1;
        vector<array<int, 3>> es;
        for (int i = 0; i < m; i++) {
            int u = rint(0, n - 2), v = rint(u + 1, n - 1);
            es.push_back({u, v, rint(-5, 10)});
        }
        MCMF f(n);
        for (auto [u, v, c] : es) f.addEdge(u, v, 1, c);
        auto [flow, cost] = f.flow(s, t);
        ll bestF = 0, bestC = 0;
        for (int mask = 0; mask < 1 << sz(es); mask++) {
            vi bal(n);
            ll c = 0;
            for (int i = 0; i < sz(es); i++) if (mask >> i & 1)
                bal[es[i][0]]--, bal[es[i][1]]++, c += es[i][2];
            bool ok = true;
            for (int v = 1; v < n - 1; v++) ok &= bal[v] == 0;
            if (!ok) continue;
            ll F = bal[t];
            if (F > bestF || (F == bestF && c < bestC)) bestF = F, bestC = c;
        }
        CHECK(flow == bestF && cost == bestC);
    }
    // flow limit
    MCMF f(3);
    f.addEdge(0, 1, 5, 2), f.addEdge(1, 2, 5, 3);
    CHECK(f.flow(0, 2, 3) == pll(3, 15));
}

void testMatching() {
    for (int it = 0; it < 1000; it++) {
        int L = rint(1, 7), R = rint(1, 7), m = rint(0, 20);
        vector<vi> g(L);
        Dinic d(L + R + 2);
        int s = L + R, t = s + 1;
        for (int i = 0; i < L; i++) d.addEdge(s, i, 1);
        for (int j = 0; j < R; j++) d.addEdge(L + j, t, 1);
        for (int i = 0; i < m; i++) {
            int u = rint(0, L - 1), v = rint(0, R - 1);
            g[u].push_back(v), d.addEdge(u, L + v, 1);
        }
        vi r(R, -1);
        int k = hopcroftKarp(g, r);
        CHECK(k == d.calc(s, t));
        int matched = 0;
        set<int> lefts;
        for (int v = 0; v < R; v++) if (r[v] != -1) {
            matched++, lefts.insert(r[v]);
            CHECK(find(all(g[r[v]]), v) != g[r[v]].end());
        }
        CHECK(matched == k && sz(lefts) == k);
    }
}

int main() {
    testShortestPaths();
    testTopoSCC();
    testTwoSat();
    testBridgesKruskal();
    testEuler();
    testFlows();
    testMatching();
    cout << "Graph: OK" << endl;
}
