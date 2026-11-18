"""One-off importer for the optional KACTL snippets (kept for reference / re-import).

python tests/import_kactl.py <path to a kactl checkout>
WARNING: the imported files were hand-edited afterwards (line widths); re-running
overwrites those edits. Use it only to re-import from scratch.

Copies the files below into content/, converts rep(i,a,b) to plain for loops, remaps
includes, and copies KACTL's own stress tests into tests/kactl/ pointing at our copies.
"""
import os, re, shutil, sys

KACTL = sys.argv[1]
TESTS_ONLY = '--tests-only' in sys.argv  # keep hand-edited content files
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# (kactl path, our path). Renamed files are remapped in includes and tests.
FILES = [
    ('data-structures/UnionFindRollback.h', 'data-structures/UnionFindRollback.h'),
    ('number-theory/ModPow.h', 'number-theory/ModPow.h'),
    ('number-theory/ModSqrt.h', 'number-theory/ModSqrt.h'),
    ('number-theory/ModSum.h', 'number-theory/ModSum.h'),
    ('number-theory/ContinuedFractions.h', 'number-theory/ContinuedFractions.h'),
    ('number-theory/FracBinarySearch.h', 'number-theory/FracBinarySearch.h'),
    ('number-theory/FastEratosthenes.h', 'number-theory/FastEratosthenes.h'),
    ('combinatorial/IntPerm.h', 'combinatorial/IntPerm.h'),
    ('combinatorial/multinomial.h', 'combinatorial/multinomial.h'),
    ('graph/BiconnectedComponents.h', 'graph/BiconnectedComponents.h'),
    ('graph/DirectedMST.h', 'graph/DirectedMST.h'),
    ('graph/EdgeColoring.h', 'graph/EdgeColoring.h'),
    ('graph/GeneralMatching.h', 'graph/GeneralMatching.h'),
    ('graph/GlobalMinCut.h', 'graph/GlobalMinCut.h'),
    ('graph/GomoryHu.h', 'graph/GomoryHu.h'),
    ('graph/LinkCutTree.h', 'graph/LinkCutTree.h'),
    ('graph/MaximalCliques.h', 'graph/MaximalCliques.h'),
    ('graph/MaximumClique.h', 'graph/MaximumClique.h'),
    ('graph/MaximumIndependentSet.h', 'graph/MaximumIndependentSet.h'),
    ('graph/DFSMatching.h', 'graph/DFSMatching.h'),
    ('graph/MinimumVertexCover.h', 'graph/MinimumVertexCover.h'),
    ('graph/PushRelabel.h', 'graph/PushRelabel.h'),
    ('graph/WeightedMatching.h', 'graph/WeightedMatching.h'),
    ('graph/MinCostMaxFlow.h', 'graph/MCMFFast.h'),
    ('numerical/BerlekampMassey.h', 'numerical/BerlekampMassey.h'),
    ('numerical/LinearRecurrence.h', 'numerical/LinearRecurrence.h'),
    ('numerical/FastFourierTransformMod.h', 'numerical/FFTMod.h'),
    ('numerical/FastSubsetTransform.h', 'numerical/FastSubsetTransform.h'),
    ('numerical/Determinant.h', 'numerical/Determinant.h'),
    ('numerical/IntDeterminant.h', 'numerical/IntDeterminant.h'),
    ('numerical/MatrixInverse.h', 'numerical/MatrixInverse.h'),
    ('numerical/MatrixInverse-mod.h', 'numerical/MatrixInverse-mod.h'),
    ('numerical/Polynomial.h', 'numerical/Polynomial.h'),
    ('numerical/PolyRoots.h', 'numerical/PolyRoots.h'),
    ('numerical/PolyInterpolate.h', 'numerical/PolyInterpolate.h'),
    ('numerical/Simplex.h', 'numerical/Simplex.h'),
    ('numerical/SolveLinearBinary.h', 'numerical/SolveLinearBinary.h'),
    ('numerical/Integrate.h', 'numerical/Integrate.h'),
    ('numerical/IntegrateAdaptive.h', 'numerical/IntegrateAdaptive.h'),
    ('numerical/GoldenSectionSearch.h', 'numerical/GoldenSectionSearch.h'),
    ('numerical/Tridiagonal.h', 'numerical/Tridiagonal.h'),
    ('numerical/HillClimbing.h', 'numerical/HillClimbing.h'),
    ('geometry/lineDistance.h', 'geometry/lineDistance.h'),
    ('geometry/LineProjectionReflection.h', 'geometry/LineProjectionReflection.h'),
    ('geometry/linearTransformation.h', 'geometry/linearTransformation.h'),
    ('geometry/Angle.h', 'geometry/Angle.h'),
    ('geometry/CircleTangents.h', 'geometry/CircleTangents.h'),
    ('geometry/CirclePolygonIntersection.h', 'geometry/CirclePolygonIntersection.h'),
    ('geometry/PolygonCenter.h', 'geometry/PolygonCenter.h'),
    ('geometry/PolygonCut.h', 'geometry/PolygonCut.h'),
    ('geometry/PolygonUnion.h', 'geometry/PolygonUnion.h'),
    ('geometry/HullDiameter.h', 'geometry/HullDiameter.h'),
    ('geometry/PointInsideHull.h', 'geometry/PointInsideHull.h'),
    ('geometry/LineHullIntersection.h', 'geometry/LineHullIntersection.h'),
    ('geometry/ManhattanMST.h', 'geometry/ManhattanMST.h'),
    ('geometry/kdTree.h', 'geometry/kdTree.h'),
    ('geometry/FastDelaunay.h', 'geometry/FastDelaunay.h'),
    ('geometry/Point3D.h', 'geometry/Point3D.h'),
    ('geometry/3dHull.h', 'geometry/3dHull.h'),
    ('geometry/PolyhedronVolume.h', 'geometry/PolyhedronVolume.h'),
    ('geometry/sphericalDistance.h', 'geometry/sphericalDistance.h'),
    ('strings/MinRotation.h', 'strings/MinRotation.h'),
    ('strings/SuffixTree.h', 'strings/SuffixTree.h'),
    ('various/IntervalContainer.h', 'various/IntervalContainer.h'),
    ('various/IntervalCover.h', 'various/IntervalCover.h'),
    ('various/ConstantIntervals.h', 'various/ConstantIntervals.h'),
    ('various/FastKnapsack.h', 'various/FastKnapsack.h'),
    ('various/KnuthDP.h', 'various/KnuthDP.h'),
    ('various/FastInput.h', 'various/FastInput.h'),
    ('various/FastMod.h', 'various/FastMod.h'),
    ('various/BumpAllocator.h', 'various/BumpAllocator.h'),
]
FIGURES = ['geometry/lineDistance.pdf', 'geometry/linearTransformation.pdf', 'geometry/PolygonCut.pdf']
# include remaps inside content files (by basename)
INCLUDE_MAP = {'FastFourierTransform.h': 'FFT.h', 'MinCostMaxFlow.h': 'MCMFFast.h',
               '../../content/geometry/Point.h': 'Point.h'}
# extra text replacements per destination file
PATCH = {'graph/MCMFFast.h': [('struct MCMF {', 'struct MCMFFast {'), ('MCMF(int N)', 'MCMFFast(int N)'),
                              # bits/extc++.h needs iconv.h, missing on MinGW; this is all it needs
                              ('// #include <bits/extc++.h> /// include-line, keep-include',
                               '#include <ext/pb_ds/priority_queue.hpp> /** keep-include */')]}

def convert_rep(src):
    out, i = [], 0
    while True:
        m = re.compile(r'(?<![\w.])rep\(').search(src, i)
        if not m:
            out.append(src[i:]); break
        out.append(src[i:m.start()])
        j, depth, args, cur = m.end(), 1, [], ''
        while depth:
            c = src[j]
            if c in '([{': depth += 1
            elif c in ')]}': depth -= 1
            if depth == 1 and c == ',': args.append(cur); cur = ''
            elif depth: cur += c
            j += 1
        args.append(cur)
        v, a, b = (x.strip() for x in args)
        out.append(f'for (int {v} = {a}; {v} < {b}; {v}++)')
        i = j
    return ''.join(out)

for src, dst in ([] if TESTS_ONLY else FILES):
    s = open(os.path.join(KACTL, 'content', src), encoding='utf8').read()
    s = convert_rep(s)
    for a, b in INCLUDE_MAP.items():
        s = s.replace(f'#include "{a}"', f'#include "{b}"')
    for a, b in PATCH.get(dst, []):
        assert a in s, (dst, a); s = s.replace(a, b)
    open(os.path.join(ROOT, 'content', dst), 'w', encoding='utf8', newline='\n').write(s)
for f in ([] if TESTS_ONLY else FIGURES):
    shutil.copy(os.path.join(KACTL, 'content', f), os.path.join(ROOT, 'content', f))

# KACTL stress tests -> tests/kactl/<dir>/, rewritten to use our files
renamed = {s.split('/')[-1]: d for s, d in FILES}
ours = {os.path.relpath(os.path.join(dp, f), os.path.join(ROOT, 'content')).replace(os.sep, '/')
        for dp, _, fs in os.walk(os.path.join(ROOT, 'content')) for f in fs if f.endswith('.h')}
TD = os.path.join(ROOT, 'tests', 'kactl')
shutil.copytree(os.path.join(KACTL, 'stress-tests', 'utilities'), os.path.join(TD, 'utilities'), dirs_exist_ok=True)
for f in os.listdir(os.path.join(TD, 'utilities')):
    p = os.path.join(TD, 'utilities', f)
    s = open(p, encoding='utf8').read().replace('"../../content/', '"../../../content/')
    if f == 'template.h':  # GCC 16 dropped cassert from bits; Windows RAND_MAX is only 32767
        s = s.replace('#include <bits/stdc++.h>\n', '#include <bits/stdc++.h>\n#include <cassert>\n'
                      '#ifdef _WIN32  // glibc-only trap, and a 31-bit rand like Linux\n'
                      '#define feenableexcept(x) 0\n'
                      'inline int rand31() { return (rand() << 16) ^ (rand() << 1) ^ rand(); }\n'
                      '#else\ninline int rand31() { return rand(); }\n#endif\n'
                      'typedef unsigned long long ull;  // KACTL gets it from ModMulLL.h\n', 1)
    open(p, 'w', encoding='utf8', newline='\n').write(s)
copied = []
# KACTL helpers that some tests need but we do not ship, and KACTL files we replaced
SUPPORT = {'data-structures/UnionFind.h', 'graph/EdmondsKarp.h', 'number-theory/Eratosthenes.h'}
TEST_MAP = {'number-theory/MillerRabin.h': 'number-theory/PollardRho.h',
            'number-theory/Factor.h': 'number-theory/PollardRho.h'}
os.makedirs(os.path.join(TD, 'support'), exist_ok=True)
for h in SUPPORT:
    open(os.path.join(TD, 'support', h.split('/')[-1]), 'w', encoding='utf8', newline='
').write(
        open(os.path.join(KACTL, 'content', h), encoding='utf8').read())
# core snippets we rewrote with a different API; tests/*.test.cpp cover them instead
SKIP_TESTS = {'data-structures/Treap.cpp', 'graph/HLD.cpp', 'graph/SCC.cpp', 'strings/AhoCorasick.cpp',
              'strings/Hashing.cpp', 'strings/KMP.cpp', 'strings/SuffixArray.cpp', 'strings/Zfunc.cpp'}
for t in SKIP_TESTS:
    if os.path.exists(os.path.join(TD, t)): os.remove(os.path.join(TD, t))
for d in os.listdir(os.path.join(KACTL, 'stress-tests')):
    if d == 'utilities': continue
    for f in os.listdir(os.path.join(KACTL, 'stress-tests', d)):
        if f'{d}/{f}' in SKIP_TESTS: continue
        path = os.path.join(KACTL, 'stress-tests', d, f)
        s = open(path, encoding='utf8').read()
        incs = re.findall(r'#include "\.\./\.\./content/([^"]+)"', s)
        known = lambda i: i in ours or i in SUPPORT or i in TEST_MAP or renamed.get(i.split('/')[-1]) in ours
        if not incs or not all(known(i) for i in incs):
            continue  # only tests whose content files are all imported here
        for inc in incs:
            if inc in SUPPORT:
                s = s.replace(f'"../../content/{inc}"', f'"../support/{inc.split("/")[-1]}"')
                continue
            target = TEST_MAP.get(inc) or (inc if inc in ours else renamed[inc.split('/')[-1]])
            s = s.replace(f'"../../content/{inc}"', f'"../../../content/{target}"')
        s = s.replace('MCMF ', 'MCMFFast ').replace('MCMF(', 'MCMFFast(')
        s = s.replace('#include <bits/extc++.h>', '#include <ext/pb_ds/priority_queue.hpp>')
        if f == 'ModSum.cpp': s = s.replace('(ll)rand()', '(ll)rand31()')
        s = s.replace('3ul<<61', '3ull<<61')  # unsigned long is 32-bit on Windows
        # newer libstdc++ calls sized delete, which must not reach free() on the static buffer
        s = s.replace('void operator delete(void*) {}', 'void operator delete(void*) {}\nvoid operator delete(void*, size_t) {}')
        # our ConvexHull / ClosestPair are templates, so the tests need KACTL's typedef
        for h in ('ConvexHull.h', 'ClosestPair.h'):
            inc = f'#include "../../../content/geometry/{h}"'
            if inc in s and 'typedef Point<ll> P;' not in s:
                s = s.replace(inc, inc + '\ntypedef Point<ll> P;', 1)
        os.makedirs(os.path.join(TD, d), exist_ok=True)
        open(os.path.join(TD, d, f), 'w', encoding='utf8', newline='\n').write(s)
        copied.append(f'{d}/{f}')
        # helper headers next to the test (e.g. MinCostMaxFlow2.h)
        for h in re.findall(r'#include "([\w.-]+\.h)"', s):
            hp = os.path.join(KACTL, 'stress-tests', d, h)
            if os.path.exists(hp):
                open(os.path.join(TD, d, h), 'w', encoding='utf8', newline='
').write(open(hp, encoding='utf8').read())
print(len(FILES), 'files imported;', len(copied), 'KACTL tests:', ', '.join(copied))
