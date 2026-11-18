/**
 * Author: Andrew He, chilli
 * Date: 2019-05-07
 * License: CC0
 * Source: folklore
 * Description: Computes the minimum circle that encloses a set of points.
 * Time: expected O(n)
 * Usage: auto [center, r] = mec(pts);  // vector<Point<double>>
 * Status: stress-tested
 */
#pragma once

#include "circumcircle.h"

pair<P, double> mec(vector<P> ps) {
	shuffle(all(ps), mt19937(time(0)));
	P o = ps[0];
	double r = 0, EPS = 1 + 1e-8;
	auto out = [&](int i) {
		return (o - ps[i]).dist() > r * EPS;
	};
	for (int i = 0; i < sz(ps); i++) if (out(i)) {
		o = ps[i], r = 0;
		for (int j = 0; j < i; j++) if (out(j)) {
			o = (ps[i] + ps[j]) / 2;
			r = (o - ps[i]).dist();
			for (int k = 0; k < j; k++) if (out(k)) {
				o = ccCenter(ps[i], ps[j], ps[k]);
				r = (o - ps[i]).dist();
			}
		}
	}
	return {o, r};
}
