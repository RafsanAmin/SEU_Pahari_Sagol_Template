#include "test.h"
#include "../content/strings/KMP.h"
#include "../content/strings/Zfunc.h"
#include "../content/strings/Hashing.h"
#include "../content/strings/Manacher.h"
#include "../content/strings/AhoCorasick.h"
#include "../content/strings/SuffixArray.h"
#include "../content/strings/SuffixAutomaton.h"

string rstr(int n, int alpha) {
    string s(n, 'a');
    for (char& c : s) c = 'a' + rint(0, alpha - 1);
    return s;
}

void testPrefixZ() {
    for (int it = 0; it < 3000; it++) {
        string s = rstr(rint(0, 30), rint(1, 3)), pat = rstr(rint(1, 4), 2);
        int n = sz(s);
        vi p = prefixFunction(s), z = zFunction(s);
        for (int i = 0; i < n; i++) {
            int best = 0;
            for (int k = 1; k <= i; k++) if (s.substr(0, k) == s.substr(i - k + 1, k)) best = k;
            CHECK(p[i] == best);
            int lcp = 0;
            if (i > 0) while (i + lcp < n && s[lcp] == s[i + lcp]) lcp++;
            CHECK(z[i] == lcp);
        }
        vi want;
        for (int i = 0; i + sz(pat) <= n; i++) if (s.substr(i, sz(pat)) == pat) want.push_back(i);
        CHECK(match(s, pat) == want);
    }
}

void testHash() {
    for (int it = 0; it < 500; it++) {
        string s = rstr(rint(1, 40), rint(1, 3)), t = rstr(rint(1, 40), 2);
        Hash hs(s), ht(t);
        for (int q = 0; q < 100; q++) {
            int l1 = rint(0, sz(s) - 1), r1 = rint(l1, sz(s) - 1);
            int l2 = rint(0, sz(t) - 1), r2 = rint(l2, sz(t) - 1);
            CHECK((hs.get(l1, r1) == ht.get(l2, r2)) == (s.substr(l1, r1 - l1 + 1) == t.substr(l2, r2 - l2 + 1)));
            int l3 = rint(0, sz(s) - 1), r3 = rint(l3, sz(s) - 1);
            CHECK((hs.get(l1, r1) == hs.get(l3, r3)) == (s.substr(l1, r1 - l1 + 1) == s.substr(l3, r3 - l3 + 1)));
        }
    }
}

void testManacher() {
    for (int it = 0; it < 3000; it++) {
        string s = rstr(rint(0, 25), rint(1, 3));
        int n = sz(s);
        auto [d1, d2] = manacher(s);
        auto isPal = [&](int l, int r) { for (; l < r; l++, r--) if (s[l] != s[r]) return false; return true; };
        for (int i = 0; i < n; i++) {
            int k = 0;
            while (i - k >= 0 && i + k < n && isPal(i - k, i + k)) k++;
            CHECK(d1[i] == k);
            k = 0;
            while (i - k - 1 >= 0 && i + k < n && isPal(i - k - 1, i + k)) k++;
            CHECK(d2[i] == k);
        }
        for (int l = 0; l < n; l++) for (int r = l; r < n; r++) {
            int len = r - l + 1;
            bool got = len % 2 ? d1[(l + r) / 2] >= (len + 1) / 2 : d2[(l + r + 1) / 2] >= len / 2;
            CHECK(got == isPal(l, r));
        }
    }
}

void testAho() {
    for (int it = 0; it < 1000; it++) {
        int k = rint(1, 6);
        vector<string> pats;
        AhoCorasick ac;
        vi ends;
        for (int i = 0; i < k; i++) pats.push_back(rstr(rint(1, 4), 2)), ends.push_back(ac.insert(pats.back()));
        ac.build();
        string t = rstr(rint(0, 40), 2);
        ll total = 0;
        vll hit(sz(ac.nxt));
        int v = 0;
        for (char c : t) v = ac.nxt[v][c - 'a'], total += ac.cnt[v], hit[v]++;
        for (int i = sz(ac.order) - 1; i >= 0; i--) hit[ac.link[ac.order[i]]] += hit[ac.order[i]];
        ll want = 0;
        for (int i = 0; i < k; i++) {
            ll c = 0;
            for (int j = 0; j + sz(pats[i]) <= sz(t); j++) c += t.substr(j, sz(pats[i])) == pats[i];
            CHECK(hit[ends[i]] == c);
            want += c;
        }
        CHECK(total == want);
    }
}

void testSuffix() {
    for (int it = 0; it < 2000; it++) {
        string s = rstr(rint(1, 30), rint(1, 3));
        int n = sz(s);
        SuffixArray S(s);
        vi want(n);
        iota(all(want), 0);
        sort(all(want), [&](int a, int b) { return s.substr(a) < s.substr(b); });
        CHECK(S.sa == want && sz(S.lcp) == n && S.lcp[0] == 0);
        for (int i = 1; i < n; i++) {
            int a = want[i], b = want[i - 1], l = 0;
            while (a + l < n && b + l < n && s[a + l] == s[b + l]) l++;
            CHECK(S.lcp[i] == l);
        }
        set<string> subs;
        for (int i = 0; i < n; i++) for (int j = i; j < n; j++) subs.insert(s.substr(i, j - i + 1));
        ll lcpSum = accumulate(all(S.lcp), 0LL);
        CHECK((ll)n * (n + 1) / 2 - lcpSum == sz(subs));
        SuffixAutomaton sa(s);
        CHECK(sa.distinct() == sz(subs));
        for (int q = 0; q < 20; q++) {
            string t = rstr(rint(1, 5), 3);
            CHECK(sa.contains(t) == (s.find(t) != string::npos));
        }
    }
}

int main() {
    testPrefixZ();
    testHash();
    testManacher();
    testAho();
    testSuffix();
    cout << "Strings: OK" << endl;
}
