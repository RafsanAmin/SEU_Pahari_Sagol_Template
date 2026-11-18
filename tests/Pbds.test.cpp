// no-debug: pb_ds is extremely slow under -D_GLIBCXX_DEBUG
#include "test.h"
#include "../content/data-structures/OrderedSet.h"
#include "../content/data-structures/HashMap.h"

void testOrderedAndHash() {
    OrderedSet<int> s;
    set<int> ref;
    for (int q = 0; q < 5000; q++) {
        int x = rint(0, 200);
        if (rint(0, 1)) s.insert(x), ref.insert(x);
        else s.erase(x), ref.erase(x);
        int y = rint(0, 200);
        CHECK((int)s.order_of_key(y) == (int)distance(ref.begin(), ref.lower_bound(y)));
        if (!ref.empty()) {
            int k = rint(0, sz(ref) - 1);
            CHECK(*s.find_by_order(k) == *next(ref.begin(), k));
        }
    }
    unordered_map<ll, int, chash> um;
    gp_hash_table<ll, int, chash> gp({}, {}, {}, {}, {1 << 4});
    map<ll, int> mp;
    for (int q = 0; q < 5000; q++) {
        ll k = (ll)rint(0, 1000) * 1000000007LL;
        int v = rint(0, 100);
        um[k] += v, gp[k] += v, mp[k] += v;
    }
    for (auto [k, v] : mp) CHECK(um[k] == v && gp[k] == v);
    CHECK(sz(um) == sz(mp));
}

int main() {
    testOrderedAndHash();
    cout << "pb_ds (OrderedSet, HashMap): OK" << endl;
}
