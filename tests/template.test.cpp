#include "test.h"

int main() {
    int a[3][4][5];
    fill_all(a, 7);
    for (auto& x : a) for (auto& y : x) for (int z : y) CHECK(z == 7);

    vector<array<ll, 3>> b[4];
    for (auto& v : b) v.resize(6);
    fill_all(b, -1LL);
    for (auto& v : b) for (auto& arr : v) for (ll z : arr) CHECK(z == -1);

    vector<vector<bool>> c(3, vector<bool>(5));
    fill_all(c, true);
    for (auto& v : c) for (bool z : v) CHECK(z);

    auto d = make_vec<ll>(2, 3, 4, -1);
    static_assert(is_same_v<decltype(d), vector<vector<vector<ll>>>>);
    CHECK(sz(d) == 2 && sz(d[0]) == 3 && sz(d[0][0]) == 4);
    for (auto& x : d) for (auto& y : x) for (ll z : y) CHECK(z == -1);
    fill_all(d, 5);
    for (auto& x : d) for (auto& y : x) for (ll z : y) CHECK(z == 5);

    auto e = make_vec<int>(5, 0);
    static_assert(is_same_v<decltype(e), vector<int>>);
    CHECK(sz(e) == 5);

    auto f = make_vec<pii>(2, 2, pii(1, 2));
    CHECK(f[1][1] == pii(1, 2));
    cout << "template: OK" << endl;
}
