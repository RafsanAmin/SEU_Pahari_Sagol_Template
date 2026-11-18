/**
 * Author: Mattias de Zalenski
 * Date: 2002-11-04
 * Description: Magic formula for the volume of a polyhedron. Faces should point outwards.
 * Usage: double vol = signedPolyVolume(pts, hull3d(pts));
 * // any face list whose items have .a .b .c indices
 * Time: O(n)
 * Status: tested
 */
#pragma once

template<class V, class L>
double signedPolyVolume(const V& p, const L& trilist) {
	double v = 0;
	for (auto i : trilist) v += p[i.a].cross(p[i.b]).dot(p[i.c]);
	return v / 6;
}
