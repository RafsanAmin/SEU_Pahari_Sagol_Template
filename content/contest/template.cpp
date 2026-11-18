#include <bits/stdc++.h>
#include <cassert>  // GCC 16+ dropped it from bits/stdc++
using namespace std;

using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
const ll INF = 1e18;

#ifdef LOCAL
#define dbg(...) cerr << "[" #__VA_ARGS__ "] =", \
	dbg_(__VA_ARGS__)
template<class... A> void dbg_(const A&... a) {
	((cerr << ' ' << a), ...) << endl;
}
#else
#define dbg(...)
#endif

// fill_all(x, v): every element of x = v, any depth/mix of
// C arrays, vector, array.  int a[5][5][5]; fill_all(a, -1);
template<class T, class V> void fill_all(T&& x, const V& v) {
	if constexpr (is_assignable_v<T, const V&>) x = v;
	else for (auto&& y : x) fill_all(y, v);
}
// make_vec<T>(d1, .., dk, init): k-dim vector of init
// auto dp = make_vec<ll>(n, m, 2, -1);  // dp[n][m][2]
template<class T, class... A> auto make_vec(size_t n, A... a) {
	if constexpr (sizeof...(A) == 1)
		return vector<T>(n, T(a...));
	else return vector(n, make_vec<T>(a...));
}

void solve() {
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int t = 1;
	// cin >> t;  // Kattis: usually ONE test per input
	while (t--) solve();
}
