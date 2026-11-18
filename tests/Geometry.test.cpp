#include "test.h"
#include "../content/geometry/Point.h"
#include "../content/geometry/sideOf.h"
#include "../content/geometry/OnSegment.h"
#include "../content/geometry/SegmentDistance.h"
#include "../content/geometry/SegmentIntersection.h"
#include "../content/geometry/lineIntersection.h"
#include "../content/geometry/AngleSort.h"
#include "../content/geometry/CircleIntersection.h"
#include "../content/geometry/CircleLine.h"
#include "../content/geometry/circumcircle.h"
#include "../content/geometry/MinimumEnclosingCircle.h"
#include "../content/geometry/PolygonArea.h"
#include "../content/geometry/InsidePolygon.h"
#include "../content/geometry/ConvexHull.h"
#include "../content/geometry/ClosestPair.h"

typedef Point<ll> PL;
PL rp(int c) { return PL(rint(-c, c), rint(-c, c)); }
P toD(PL p) { return P(p.x, p.y); }
const double EPS = 1e-6;

bool segTouch(PL a, PL b, PL c, PL d) {  // brute: do closed segments intersect?
    auto o = [](PL p, PL q, PL r) { return sgn(p.cross(q, r)); };
    auto onS = [](PL p, PL q, PL r) {  // r on segment pq
        return p.cross(q, r) == 0 && min(p.x, q.x) <= r.x && r.x <= max(p.x, q.x)
            && min(p.y, q.y) <= r.y && r.y <= max(p.y, q.y);
    };
    if (onS(a, b, c) || onS(a, b, d) || onS(c, d, a) || onS(c, d, b)) return true;
    return o(a, b, c) * o(a, b, d) < 0 && o(c, d, a) * o(c, d, b) < 0;
}

void testSegments() {
    for (int it = 0; it < 20000; it++) {
        PL a = rp(5), b = rp(5), c = rp(5), d = rp(5);
        if (a == b || c == d) continue;
        auto res = segInter(toD(a), toD(b), toD(c), toD(d));
        CHECK(!res.empty() == segTouch(a, b, c, d));
        for (P p : res) CHECK(segDist(toD(a), toD(b), p) < EPS && segDist(toD(c), toD(d), p) < EPS);
        // segment distance vs sampling
        P q = toD(rp(6));
        double best = 1e18;
        for (int s = 0; s <= 2000; s++) {
            P x = toD(a) + (toD(b) - toD(a)) * (s / 2000.0);
            best = min(best, (x - q).dist());
        }
        CHECK(fabs(segDist(toD(a), toD(b), q) - best) < 1e-2);
        CHECK(onSegment(a, b, PL(q.x, q.y)) == (segDist(toD(a), toD(b), q) < EPS));
        CHECK(sideOf(a, b, c) == sgn((b - a).cross(c - a)));
        auto [kind, p] = lineInter(toD(a), toD(b), toD(c), toD(d));
        if (kind == 1) {
            CHECK(fabs(toD(a).cross(toD(b), p)) < 1e-6 && fabs(toD(c).cross(toD(d), p)) < 1e-6);
        } else {
            CHECK((b - a).cross(d - c) == 0);
            CHECK((kind == -1) == (a.cross(b, c) == 0));
        }
    }
}

void testAngleSort() {
    for (int it = 0; it < 2000; it++) {
        int n = rint(1, 20);
        vector<PL> v;
        while (sz(v) < n) { PL p = rp(4); if (!(p == PL(0, 0))) v.push_back(p); }
        polarSort(v);
        auto ang = [](PL p) { double a = atan2((double)p.y, (double)p.x); return a < 0 ? a + 2 * acos(-1.0) : a; };
        for (int i = 1; i < n; i++) CHECK(ang(v[i - 1]) <= ang(v[i]) + 1e-12);
    }
}

void testCircles() {
    for (int it = 0; it < 20000; it++) {
        P a = toD(rp(5)), b = toD(rp(5));
        double r1 = rint(1, 6), r2 = rint(1, 6);
        if (a == b) continue;
        pair<P, P> out;
        double d = (a - b).dist();
        bool want = d <= r1 + r2 + 1e-9 && d >= fabs(r1 - r2) - 1e-9;
        bool got = circleInter(a, b, r1, r2, &out);
        CHECK(got == want);
        if (got) for (P p : {out.first, out.second})
            CHECK(fabs((p - a).dist() - r1) < EPS && fabs((p - b).dist() - r2) < EPS);
        // circle and line through two lattice points
        P c = toD(rp(5)), l1 = toD(rp(5)), l2 = toD(rp(5));
        if (l1 == l2) continue;
        auto pts = circleLine(c, r1, l1, l2);
        double ld = fabs(l1.cross(l2, c)) / (l2 - l1).dist();
        if (fabs(ld - r1) > 1e-9) CHECK(sz(pts) == (ld < r1 ? 2 : 0));
        for (P p : pts) CHECK(fabs((p - c).dist() - r1) < EPS && fabs(l1.cross(l2, p)) < 1e-6);
    }
    for (int it = 0; it < 2000; it++) {
        P A = toD(rp(10)), B = toD(rp(10)), Cc = toD(rp(10));
        if (fabs((B - A).cross(Cc - A)) < 0.5) continue;
        P o = ccCenter(A, B, Cc);
        double r = ccRadius(A, B, Cc);
        for (P p : {A, B, Cc}) CHECK(fabs((p - o).dist() - r) < EPS);
    }
    for (int it = 0; it < 300; it++) {
        int n = rint(1, 12);
        vector<P> ps;
        for (int i = 0; i < n; i++) ps.push_back(toD(rp(10)));
        auto [o, r] = mec(ps);
        for (P p : ps) CHECK((p - o).dist() <= r + 1e-6);
        // brute: best circle through 2 or 3 points
        double best = n == 1 ? 0 : 1e18;
        auto tryC = [&](P c, double rr) {
            for (P p : ps) if ((p - c).dist() > rr + 1e-7) return;
            best = min(best, rr);
        };
        for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) {
            tryC((ps[i] + ps[j]) / 2, (ps[i] - ps[j]).dist() / 2);
            for (int k = j + 1; k < n; k++)
                if (fabs((ps[j] - ps[i]).cross(ps[k] - ps[i])) > 1e-9)
                    tryC(ccCenter(ps[i], ps[j], ps[k]), ccRadius(ps[i], ps[j], ps[k]));
        }
        if (n > 1 && best > 1e17) best = 0;  // all points equal
        CHECK(fabs(r - best) < 1e-6);
    }
}

void testPolygons() {
    for (int it = 0; it < 3000; it++) {
        int n = rint(1, 15);
        vector<PL> pts;
        for (int i = 0; i < n; i++) pts.push_back(rp(6));
        vector<PL> h = convexHull(pts);
        int k = sz(h);
        // hull: strictly convex, ccw, contains every point
        if (k >= 3) {
            for (int i = 0; i < k; i++) CHECK(h[i].cross(h[(i + 1) % k], h[(i + 2) % k]) > 0);
            for (PL p : pts) for (int i = 0; i < k; i++) CHECK(h[i].cross(h[(i + 1) % k], p) >= 0);
            // area: fan triangulation from h[0]
            ll fan = 0;
            for (int i = 1; i + 1 < k; i++) fan += h[0].cross(h[i], h[i + 1]);
            CHECK(polygonArea2(h) == fan && fan > 0);
            vector<PL> rev(h.rbegin(), h.rend());
            CHECK(polygonArea2(rev) == -fan);
            for (int q = 0; q < 20; q++) {
                PL p = rp(7);
                bool strictIn = true, onB = false;
                for (int i = 0; i < k; i++) {
                    ll c = h[i].cross(h[(i + 1) % k], p);
                    if (c <= 0) strictIn = false;
                    if (onSegment(h[i], h[(i + 1) % k], p)) onB = true;
                }
                CHECK(inPolygon(h, p, true) == strictIn);
                CHECK(inPolygon(h, p, false) == (strictIn || onB));
            }
        }
        set<PL> distinct(all(pts));
        if (sz(distinct) == 1) CHECK(k == 1);
        for (PL p : h) CHECK(distinct.count(p));
        if (n >= 2) {
            auto [a, b] = closest(pts);
            ll best = LLONG_MAX;
            for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) best = min(best, (pts[i] - pts[j]).dist2());
            CHECK((a - b).dist2() == best);
        }
    }
}

int main() {
    testSegments();
    testAngleSort();
    testCircles();
    testPolygons();
    cout << "Geometry: OK" << endl;
}
