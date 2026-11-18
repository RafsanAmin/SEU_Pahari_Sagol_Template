/**
 * Author: Ulf Lundstrom
 * Date: 2009-03-21
 * License: CC0
 * Source: tinyKACTL
 * Description: Returns twice the signed area of a polygon.
 *  Clockwise enumeration gives negative area. Watch out for overflow if using int as T!
 * Usage: ll a2 = polygonArea2(poly);  // area = abs(a2) / 2.0
 * Time: O(n)
 * Status: Stress-tested and tested on kattis:polygonarea
 */
#pragma once

#include "Point.h"

template<class T>
T polygonArea2(vector<Point<T>>& v) {
	T a = v.back().cross(v[0]);
	for (int i = 0; i < sz(v) - 1; i++) a += v[i].cross(v[i+1]);
	return a;
}
