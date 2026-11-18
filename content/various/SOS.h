/**
 * Author: Team
 * License: CC0
 * Description: Sum over subsets: f[m] becomes the sum of f[s]
 * over all submasks s of m, in place. Superset sums: use
 * \texttt{!(m >> b \& 1)} and \texttt{f[m | 1 << b]}. Undo
 * (Mobius) with -= instead of +=. Enumerate submasks of m:
 * \texttt{for (s = m; s; s = (s - 1) \& m)}, $3^n$ in total.
 * Usage: vll f(1 << B); sos(f, B);
 * Time: O(B 2^B)
 * Status: stress-tested
 */
#pragma once

void sos(vll& f, int B) {
	for (int b = 0; b < B; b++)
		for (int m = 0; m < 1 << B; m++)
			if (m >> b & 1) f[m] += f[m ^ 1 << b];
}
