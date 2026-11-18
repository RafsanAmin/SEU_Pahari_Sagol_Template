#include "test.h"
#include "../content/numerical/Matrix.h"
#include "../content/numerical/Gauss.h"
#include "../content/numerical/XorBasis.h"
#include "../content/numerical/NTT.h"
#include "../content/numerical/FFT.h"
#include "../content/numerical/TernarySearch.h"

void testMatrix() {
    Mat f = {{1, 1}, {1, 0}};
    Mint a = 0, b = 1;  // F(0), F(1)
    for (int n = 0; n < 200; n++) {
        CHECK(matPow(f, n)[0][1] == a);
        Mint c = a + b; a = b, b = c;
    }
    for (int it = 0; it < 200; it++) {
        int n = rint(1, 5), e = rint(0, 10);
        Mat m(n, vector<Mint>(n));
        for (auto& row : m) for (auto& x : row) x = rint(0, 1000);
        Mat r(n, vector<Mint>(n));
        for (int i = 0; i < n; i++) r[i][i] = 1;
        for (int i = 0; i < e; i++) r = r * m;
        CHECK(matPow(m, e) == r);
    }
}

int rankMint(vector<vector<Mint>> a) {
    int r = 0, n = sz(a), m = sz(a[0]);
    for (int c = 0; c < m && r < n; c++) {
        int p = -1;
        for (int i = r; i < n; i++) if (a[i][c].v) { p = i; break; }
        if (p < 0) continue;
        swap(a[p], a[r]);
        for (int i = r + 1; i < n; i++) {
            Mint f = a[i][c] / a[r][c];
            for (int j = c; j < m; j++) a[i][j] -= f * a[r][j];
        }
        r++;
    }
    return r;
}

void testGauss() {
    for (int it = 0; it < 3000; it++) {
        int n = rint(1, 5), m = rint(1, 5);
        vector<vector<Mint>> A(n, vector<Mint>(m)), aug(n, vector<Mint>(m + 1));
        for (auto& row : A) for (auto& x : row) x = rint(0, 3) ? rint(0, 2) : 0;
        vector<Mint> x0(m);
        for (auto& x : x0) x = rint(0, 5);
        bool consistent = rint(0, 1);
        for (int i = 0; i < n; i++) {
            Mint s = 0;
            for (int j = 0; j < m; j++) s += A[i][j] * x0[j], aug[i][j] = A[i][j];
            aug[i][m] = consistent ? s : Mint(rint(0, 5));
        }
        int rA = rankMint(A), rAug = rankMint(aug);
        vector<Mint> x;
        int res = gauss(aug, x);
        int want = rAug > rA ? 0 : (rA == m ? 1 : 2);
        CHECK(res == want);
        if (res) for (int i = 0; i < n; i++) {
            Mint s = 0;
            for (int j = 0; j < m; j++) s += A[i][j] * x[j];
            CHECK(s == aug[i][m]);
        }
        // the same system in doubles
        vector<vector<double>> ad(n, vector<double>(m + 1));
        for (int i = 0; i < n; i++) for (int j = 0; j <= m; j++) ad[i][j] = aug[i][j].v;
        vector<double> xd;
        int resd = gauss(ad, xd);
        if (consistent) {
            // integer system with an integer solution: double result must solve it too
            CHECK(resd == (rankMint(A) == m ? 1 : resd));
            CHECK(resd != 0);
            for (int i = 0; i < n; i++) {
                double s = 0;
                for (int j = 0; j < m; j++) s += ad[i][j] * xd[j];
                CHECK(fabs(s - ad[i][m]) < 1e-6);
            }
        }
    }
}

void testXorBasis() {
    for (int it = 0; it < 2000; it++) {
        XorBasis b;
        set<ll> span = {0};
        for (int q = 0; q < rint(0, 6); q++) {
            ll x = rint(0, 63);
            bool newVec = !span.count(x);
            CHECK(b.insert(x) == newVec);
            set<ll> nxt = span;
            for (ll y : span) nxt.insert(y ^ x);
            span = nxt;
        }
        CHECK(sz(span) == (1 << b.rank));
        for (ll y = 0; y < 64; y++) CHECK(b.canMake(y) == (bool)span.count(y));
        ll x = rint(0, 63), best = 0;
        for (ll y : span) best = max(best, x ^ y);
        CHECK(b.maxXor(x) == best);
    }
    XorBasis big;
    big.insert((1LL << 62) | 5), big.insert(3);
    CHECK(big.maxXor() == ((1LL << 62) | 6));
}

void testConv() {
    for (int it = 0; it < 300; it++) {
        int n = rint(0, 70), m = rint(0, 70);
        vll a(n), b(m);
        for (ll& x : a) x = rint(0, nttMod - 1) ;
        for (ll& x : b) x = rint(0, nttMod - 1);
        vll want(n && m ? n + m - 1 : 0);
        for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) want[i + j] = (want[i + j] + a[i] * b[j]) % nttMod;
        CHECK(conv(a, b) == want);
        vector<double> da(n), db(m);
        vll ia(n), ib(m);
        for (int i = 0; i < n; i++) da[i] = ia[i] = rint(-1000, 1000);
        for (int j = 0; j < m; j++) db[j] = ib[j] = rint(-1000, 1000);
        vector<double> c = convDouble(da, db);
        CHECK(sz(c) == sz(want));
        for (int k = 0; k < sz(c); k++) {
            ll s = 0;
            for (int i = 0; i < n; i++) if (k - i >= 0 && k - i < m) s += ia[i] * ib[k - i];
            CHECK(llround(c[k]) == s);
        }
    }
    // large size sanity (static root tables grow)
    vll a(1 << 15, 1), b(1 << 15, 1);
    vll c = conv(a, b);
    CHECK(c[(1 << 15) - 1] == (1 << 15) && c[0] == 1);
}

void testTernary() {
    for (int it = 0; it < 5000; it++) {
        int n = rint(1, 30), peak = rint(0, n - 1);
        vi a(n);
        a[peak] = 100;
        for (int i = peak - 1; i >= 0; i--) a[i] = a[i + 1] - rint(1, 5);
        for (int i = peak + 1; i < n; i++) a[i] = a[i - 1] - rint(0, 5);
        int got = ternSearch(0, n - 1, [&](int i) { return a[i]; });
        CHECK(got == peak);
    }
    for (int it = 0; it < 100; it++) {
        double c = rint(-1000, 1000) / 7.0;
        double x = ternSearchReal(-500, 500, [&](double t) { return -(t - c) * (t - c); });
        CHECK(fabs(x - c) < 1e-6);
    }
}

int main() {
    testMatrix();
    testGauss();
    testXorBasis();
    testConv();
    testTernary();
    cout << "Numerical: OK" << endl;
}
