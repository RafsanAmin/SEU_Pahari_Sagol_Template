/**
 * Author: Team
 * License: CC0
 * Description: Sorts vectors by polar angle, counter-clockwise
 * starting from the positive x-axis, angles in [0, 2pi).
 * (0, 0) is not allowed. Exact for Point<ll>.
 * Usage: polarSort(v);
 * Time: O(N \log N)
 * Status: stress-tested
 */
#pragma once

#include "Point.h"

template<class P> bool upperHalf(P p) {
	return p.y > 0 || (p.y == 0 && p.x > 0);
}
template<class P> void polarSort(vector<P>& v) {
	sort(all(v), [](P a, P b) {
		if (upperHalf(a) != upperHalf(b)) return upperHalf(a);
		return a.cross(b) > 0;
	});
}
