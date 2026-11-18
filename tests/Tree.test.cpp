#include "test.h"
#include "../content/data-structures/SegTree.h"
#include "../content/data-structures/LazySegTree.h"
#include "../content/graph/LCA.h"
#include "../content/graph/HLD.h"
#include "../content/graph/Centroid.h"
#include "../content/graph/Reroot.h"

// path from u to v (inclusive) via parents from a BFS rooted at root
vi pathNodes(const vector<vi>& g, int root, int u, int v) {
    int n = sz(g);
    vi par(n, -1), dep = bfsDist(g, root);
    for (int x = 0; x < n; x++) for (int y : g[x]) if (dep[y] == dep[x] + 1) par[y] = x;
    vi a, b;
    while (u != v) {
        if (dep[u] >= dep[v]) a.push_back(u), u = par[u];
        else b.push_back(v), v = par[v];
    }
    a.push_back(u);
    a.insert(a.end(), b.rbegin(), b.rend());
    return a;
}

void testLCA() {
    for (int it = 0; it < 300; it++) {
        int n = rint(1, 40), root = rint(0, n - 1);
        auto g = randomTree(n);
        LCA L(g, root);
        HLD h(g, false, root);
        vi dep = bfsDist(g, root);
        for (int q = 0; q < 50; q++) {
            int a = rint(0, n - 1), b = rint(0, n - 1);
            vi p = pathNodes(g, root, a, b);
            int want = p[0];
            for (int x : p) if (dep[x] < dep[want]) want = x;
            CHECK(L.lca(a, b) == want && h.lca(a, b) == want);
            CHECK(L.dist(a, b) == sz(p) - 1);
            int k = rint(0, dep[a]);
            vi up = pathNodes(g, root, a, root);
            CHECK(L.kth(a, k) == up[k]);
        }
    }
}

void testHLD() {
    for (int it = 0; it < 300; it++) {
        int n = rint(1, 40), root = rint(0, n - 1);
        bool edges = it % 2;
        auto g = randomTree(n);
        HLD h(g, edges, root);
        // positions form a permutation
        vi seen(n);
        for (int v = 0; v < n; v++) seen[h.pos[v]]++;
        CHECK(count(all(seen), 1) == n);
        vll val(n);  // node value, or value of edge (par, v)
        LazySegTree st(n);
        vi dep = bfsDist(g, root);
        for (int q = 0; q < 60; q++) {
            int a = rint(0, n - 1), b = rint(0, n - 1), type = rint(0, 2);
            vi p = pathNodes(g, root, a, b);
            int top = p[0];
            for (int x : p) if (dep[x] < dep[top]) top = x;
            if (type == 0) {
                ll x = rint(-5, 5);
                for (int v : p) if (!edges || v != top) val[v] += x;
                h.path(a, b, [&](int l, int r) { st.update(l, r, {x}); });
            } else if (type == 1) {
                ll want = 0, got = 0;
                for (int v : p) if (!edges || v != top) want += val[v];
                h.path(a, b, [&](int l, int r) { CHECK(l <= r); got += st.query(l, r).sum; });
                CHECK(got == want);
            } else {
                vi d = bfsDist(g, a);
                ll want = 0;
                // subtree of a when rooted at root: nodes whose root-path contains a
                for (int v = 0; v < n; v++) {
                    vi up = pathNodes(g, root, v, root);
                    if (find(all(up), a) != up.end() && (!edges || v != a)) want += val[v];
                }
                auto [l, r] = h.subtree(a);
                CHECK((l <= r ? st.query(l, r).sum : 0) == want);
            }
        }
    }
}

void testCentroid() {
    for (int it = 0; it < 300; it++) {
        int n = rint(1, 60), K = rint(1, 6);
        auto g = randomTree(n);
        Centroid cd(g);
        int maxLvl = *max_element(all(cd.lvl));
        CHECK((1 << maxLvl) <= n);
        // count pairs at distance exactly K, the standard centroid way
        ll got = 0;
        for (int c = 0; c < n; c++) {
            vll total(n + K + 1);
            total[0] = 1;
            for (int s : g[c]) if (cd.lvl[s] > cd.lvl[c]) {
                vll here(n + K + 1);
                function<void(int, int, int)> dfs = [&](int u, int p, int d) {
                    here[d]++;
                    for (int v : g[u]) if (v != p && cd.lvl[v] > cd.lvl[c]) dfs(v, u, d + 1);
                };
                dfs(s, c, 1);
                for (int d = 0; d <= K; d++) got += here[d] * total[K - d];
                for (int d = 0; d <= n; d++) total[d] += here[d];
            }
        }
        ll want = 0;
        for (int u = 0; u < n; u++) { vi d = bfsDist(g, u); for (int v = u + 1; v < n; v++) want += d[v] == K; }
        CHECK(got == want);
        // centroid property: each component has every child part <= half
        for (int c = 0; c < n; c++) {
            int comp = 0;
            vi big;
            for (int s : g[c]) if (cd.lvl[s] > cd.lvl[c]) {
                int cnt = 0;
                function<void(int, int)> dfs = [&](int u, int p) {
                    cnt++;
                    for (int v : g[u]) if (v != p && cd.lvl[v] > cd.lvl[c]) dfs(v, u);
                };
                dfs(s, c);
                big.push_back(cnt), comp += cnt;
            }
            for (int b : big) CHECK(2 * b <= comp + 1);
        }
    }
}

void testReroot() {
    for (int it = 0; it < 300; it++) {
        int n = rint(1, 50);
        auto g = randomTree(n);
        auto ans = reroot(g);
        for (int v = 0; v < n; v++) {
            vi d = bfsDist(g, v);
            CHECK(ans[v].second == accumulate(all(d), 0LL));
            CHECK(ans[v].first == n - 1);
        }
    }
}

int main() {
    testLCA();
    testHLD();
    testCentroid();
    testReroot();
    cout << "Trees: OK" << endl;
}
