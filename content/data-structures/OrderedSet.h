/**
 * Author: Team (adapted from KACTL)
 * License: CC0
 * Description: std::set with k-th element and rank queries.
 * Everything else (insert, erase, find, lower\_bound, size,
 * iteration) works like std::set. find\_by\_order(k) returns
 * end() if k >= size. For a map use tree<K, V, less<K>, ...>
 * with the same tags; m[key] works.
 * Usage: OrderedSet<int> s; s.insert(x); s.erase(x);
 * int kth = *s.find_by_order(k);  // k-th smallest, 0-indexed
 * int less = s.order_of_key(x);   // number of elements < x
 * // multiset: store (value, unique id)
 * OrderedSet<pii> m; m.insert({x, id++});
 * m.erase(m.lower_bound({x, INT_MIN}));  // erase one copy of x
 * m.order_of_key({x, INT_MIN});          // number < x
 * Time: O(\log N) per operation.
 * Status: tested
 */
#pragma once

#include <ext/pb_ds/assoc_container.hpp> /** keep-include */
#include <ext/pb_ds/tree_policy.hpp> /** keep-include */
using namespace __gnu_pbds;
template<class T> using OrderedSet = tree<T, null_type,
	less<T>, rb_tree_tag, tree_order_statistics_node_update>;
