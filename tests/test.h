// Shared test header: pulls in the real contest template (renaming its main) so tests
// always use exactly the aliases and utilities that get printed.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#define main template_main_unused
#include "../content/contest/template.cpp"
#undef main
#pragma GCC diagnostic pop

mt19937 test_rng(12345);
int rint(int lo, int hi) { return uniform_int_distribution<int>(lo, hi)(test_rng); }

#define CHECK(cond) do { if (!(cond)) { \
    cerr << "FAILED: " #cond " at " __FILE__ ":" << __LINE__ << endl; exit(1); } } while (0)

// random tree on n nodes as adjacency lists
vector<vi> randomTree(int n) {
    vector<vi> g(n);
    for (int i = 1; i < n; i++) {
        int p = rint(0, i - 1);
        g[i].push_back(p), g[p].push_back(i);
    }
    vi perm(n);
    iota(all(perm), 0);
    shuffle(all(perm), test_rng);
    vector<vi> h(n);
    for (int u = 0; u < n; u++) for (int v : g[u]) h[perm[u]].push_back(perm[v]);
    return h;
}
// BFS distances in an unweighted graph
vi bfsDist(const vector<vi>& g, int s) {
    vi d(sz(g), -1); d[s] = 0;
    queue<int> q; q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) if (d[v] < 0) d[v] = d[u] + 1, q.push(v);
    }
    return d;
}
