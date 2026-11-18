#include <bits/stdc++.h>
#include <cassert>
#ifdef _WIN32  // glibc-only trap, and a 31-bit rand like Linux
#define feenableexcept(x) 0
inline int rand31() { return (rand() << 16) ^ (rand() << 1) ^ rand(); }
#else
inline int rand31() { return rand(); }
#endif
typedef unsigned long long ull;  // KACTL gets it from ModMulLL.h
using namespace std;

#define rep(i, from, to) for (int i = from; i < (to); ++i)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
