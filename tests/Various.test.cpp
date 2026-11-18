#include "test.h"
#include "../content/various/LIS.h"
#include "../content/various/DivideConquerDP.h"
#include "../content/various/SOS.h"
#include "../content/various/DigitDP.h"
#include "../content/various/Misc.h"

void testLIS() {
    for (int it = 0; it < 3000; it++) {
        int n = rint(0, 15);
        vi a(n);
        for (int& x : a) x = rint(0, 6);
        vi dp(n, 1);
        int best = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < i; j++) if (a[j] < a[i]) dp[i] = max(dp[i], dp[j] + 1);
            best = max(best, dp[i]);
        }
        vi idx = lis(a);
        CHECK(sz(idx) == best);
        for (int i = 1; i < sz(idx); i++) CHECK(idx[i - 1] < idx[i] && a[idx[i - 1]] < a[idx[i]]);
    }
}

vll pre;
ll cost(int j, int i) { ll s = pre[i] - pre[j]; return s * s; }  // (sum of a[j..i-1])^2
void testDnC() {
    for (int it = 0; it < 500; it++) {
        int n = rint(1, 25), k = rint(1, n);
        vll a(n);
        for (ll& x : a) x = rint(0, 20);
        pre.assign(n + 1, 0);
        for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + a[i];
        // brute
        vector<vll> dp(k + 1, vll(n + 1, INF));
        dp[0][0] = 0;
        for (int g = 1; g <= k; g++) for (int i = 1; i <= n; i++)
            for (int j = 0; j < i; j++) if (dp[g - 1][j] < INF)
                dp[g][i] = min(dp[g][i], dp[g - 1][j] + cost(j, i));
        // layered D&C exactly as in Usage
        prv.assign(n + 1, INF), prv[0] = 0;
        for (int g = 1; g <= k; g++) {
            cur.assign(n + 1, INF);
            compute(1, n, 0, n - 1);
            swap(prv, cur);
        }
        CHECK(prv[n] == dp[k][n]);
    }
}

void testSOS() {
    for (int it = 0; it < 200; it++) {
        int B = rint(0, 8);
        vll f(1 << B);
        for (ll& x : f) x = rint(-10, 10);
        vll g = f;
        sos(g, B);
        for (int m = 0; m < 1 << B; m++) {
            ll s = 0;
            for (int t = 0; t < 1 << B; t++) if ((t & m) == t) s += f[t];
            CHECK(g[m] == s);
        }
    }
}

void testDigitDP() {
    for (int K = 1; K <= 7; K++) {
        ll c = 0;
        for (ll x = 0; x <= 3000; x++) {
            ll s = 0;
            for (ll y = x; y; y /= 10) s += y % 10;
            c += s % K == 0;
            if (x % 37 == 0 || x == 3000) CHECK(countUpTo(x, K) == c);
        }
        CHECK(countUpTo(-1, K) == 0);
    }
}

void testMisc() {
    for (int it = 0; it < 500; it++) {
        int n = rint(1, 20);
        vll a(n), orig;
        for (ll& x : a) x = rint(-5, 5);
        orig = a;
        vll v = compress(a);
        CHECK(is_sorted(all(v)) && adjacent_find(all(v)) == v.end());
        for (int i = 0; i < n; i++) CHECK(v[a[i]] == orig[i]);
        ll lo = rint(-50, 50), hi = rint(lo, lo + 60), t = rint(lo - 5, hi + 5);
        ll want = max(lo, min(hi + 1, t));
        CHECK(firstTrue(lo, hi, [&](ll x) { return x >= t; }) == want);
        ll p = rint(-100, 100), q = rint(-10, 10);
        if (q != 0) {
            CHECK(floorDiv(p, q) == (ll)floor((double)p / q));
            CHECK(ceilDiv(p, q) == (ll)ceil((double)p / q));
        }
        ll r = rnd(-3, 3);
        CHECK(r >= -3 && r <= 3);
    }
    FILE* f = freopen("bin/print128.txt", "w", stdout);
    CHECK(f);
    print128((__int128)-123456789012345678LL * 1000);
    putchar(' ');
    print128(0);
    fflush(stdout);
    freopen("CON", "w", stdout);
    ifstream in("bin/print128.txt");
    string s1, s2;
    in >> s1 >> s2;
    CHECK(s1 == "-123456789012345678000" && s2 == "0");
}

int main() {
    testLIS();
    testDnC();
    testSOS();
    testDigitDP();
    testMisc();
    cerr << "Various: OK" << endl;
}
