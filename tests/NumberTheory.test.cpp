#include "test.h"
#include "../content/number-theory/ModInt.h"
#include "../content/number-theory/Euclid.h"
#include "../content/number-theory/CRT.h"
#include "../content/number-theory/Sieve.h"
#include "../content/number-theory/Factorize.h"
#include "../content/number-theory/PollardRho.h"
#include "../content/number-theory/ModLog.h"
#include "../content/combinatorial/Combinatorics.h"

void testMint() {
    for (int it = 0; it < 10000; it++) {
        ll a = rint(-1000000000, 1000000000), b = rint(1, 1000000000);
        Mint x = a, y = b;
        ll ra = ((a % MOD) + MOD) % MOD, rb = b % MOD;
        CHECK((x + y).v == (ra + rb) % MOD);
        CHECK((x - y).v == ((ra - rb) % MOD + MOD) % MOD);
        CHECK((x * y).v == ra * rb % MOD);
        CHECK((x / y) * y == x);
        CHECK((-x + x).v == 0);
        ll e = rint(0, 50);
        ll p = 1;
        for (int i = 0; i < e; i++) p = p * ra % MOD;
        CHECK(x.pow(e).v == p);
        CHECK(power(a, e, MOD) == p || (a < 0));
    }
    stringstream ss("12345678901");
    Mint z; ss >> z;
    CHECK(z.v == 12345678901LL % MOD);
}

void testEuclidCRT() {
    for (int it = 0; it < 20000; it++) {
        ll a = rint(0, 1000000), b = rint(0, 1000000), x, y;
        ll g = euclid(a, b, x, y);
        CHECK(g == gcd(a, b) && a * x + b * y == g);
        ll m = rint(1, 1000);
        ll inv = modInverse(a, m);
        if (gcd(a, m) == 1) CHECK(inv >= 0 && inv < m && a % m * inv % m == 1 % m);
        else CHECK(inv == -1);
        ll m1 = rint(1, 300), m2 = rint(1, 300), r1 = rint(0, m1 - 1), r2 = rint(0, m2 - 1);
        ll l = m1 / gcd(m1, m2) * m2, want = -1;
        for (ll t = 0; t < l; t++) if (t % m1 == r1 && t % m2 == r2) { want = t; break; }
        CHECK(crt(r1, m1, r2, m2) == want);
    }
}

void testDiophantine() {
    for (int it = 0; it < 200000; it++) {
        ll a = rint(1, 60), b = rint(1, 60), c = rint(-300, 300), x, y;
        bool got = diophantine(a, b, c, x, y);
        ll g = gcd(a, b);
        CHECK(got == (c % g == 0));
        if (got) CHECK(a * x + b * y == c && 0 <= x && x < b / g);
    }
    ll a = 999999937, b = 1000000007, c = 1000000000000000000LL, x, y;
    CHECK(diophantine(a, b, c, x, y));
    CHECK((__int128)a * x + (__int128)b * y == c && x >= 0 && x < b);
}

void testSieveFactor() {
    int N = 20000;
    sieve(N);
    for (int x = 1; x <= N; x++) {
        // brute phi, mu, spf
        int ph = 0;
        for (int i = 1; i <= x; i++) ph += gcd(i, x) == 1;
        if (x <= 3000) CHECK(phi[x] == ph);
        auto f = factorize(x);
        CHECK(factorSmall(x) == vector<pii>(all(f)) || x == 1);
        int m = 1;
        for (auto [p, e] : f) m = e > 1 ? 0 : -m;
        CHECK(mu[x] == m);
        if (x > 1) CHECK(spf[x] == f[0].first);
        CHECK(phiOf(x) == phi[x]);
        vll d;
        for (int i = 1; i <= x; i++) if (x % i == 0) d.push_back(i);
        if (x <= 5000) CHECK(divisors(x) == d);
        bool pr = x > 1 && sz(f) == 1 && f[0].second == 1;
        CHECK(isPrime(x) == pr);
    }
    CHECK(sz(primes) == 2262);  // primes below 20000
    sieve(100);
    CHECK(sz(primes) == 25);
}

void testPollard() {
    vector<ull> tests = {1, 2, 4, 12, 999999999999999989ULL, 1000000007ULL * 998244353ULL,
                         (1ULL << 62), 352523ULL * 352817ULL, 4295098369ULL, 999999999999999999ULL};
    mt19937_64 r(7);
    for (int i = 0; i < 300; i++) tests.push_back(r() % (ull)1e18 + 1);
    for (ull n : tests) {
        auto f = factor(n);
        ull prod = 1;
        for (ull p : f) { CHECK(isPrime(p)); prod *= p; }
        CHECK(prod == n);
    }
    CHECK(isPrime(999999999999999989ULL) && !isPrime(999999999999999999ULL));
    CHECK(isPrime(998244353) && !isPrime(561) && !isPrime(1));
}

void testModLog() {
    for (int m = 1; m < 120; m++) for (int a = 0; a < m; a++) for (int b = 0; b < m; b++) {
        ll want = -1, e = a % m;
        for (int x = 1; x <= m + 2; x++, e = e * a % m)
            if (e == b % m) { want = x; break; }
        CHECK(modLog(a, b, m) == want);
    }
}

void testCombinatorics() {
    initFact(1000);
    vector<vector<Mint>> pas(60, vector<Mint>(60));
    for (int n = 0; n < 60; n++) for (int k = 0; k <= n; k++)
        pas[n][k] = k == 0 || k == n ? Mint(1) : pas[n - 1][k - 1] + pas[n - 1][k];
    for (int n = 0; n < 60; n++) for (int k = -2; k <= n + 2; k++)
        CHECK(C(n, k) == (k < 0 || k > n ? Mint(0) : pas[n][k]));
    ll cat[] = {1, 1, 2, 5, 14, 42, 132, 429, 1430, 4862, 16796};
    for (int i = 0; i <= 10; i++) CHECK(catalan(i).v == cat[i]);
    // lucas: C(n, k) for huge n via the real formula on a check that reduces to small cases
    CHECK(lucas(5, 2) == C(5, 2));
    CHECK(lucas(MOD + 5, 2) == C(5, 2));  // C(p+5, 2) = C(1,0) * C(5,2) mod p
}

int main() {
    testMint();
    testEuclidCRT();
    testDiophantine();
    testSieveFactor();
    testPollard();
    testModLog();
    testCombinatorics();
    cout << "Number theory: OK" << endl;
}
