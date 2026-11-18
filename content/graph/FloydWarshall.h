/**
 * Author: Team
 * License: CC0
 * Description: All-pairs shortest paths. Input d[i][j] = edge
 * weight or INF, d[i][i] = 0. After: d[i][j] = distance,
 * -INF if a negative cycle is on some i-j path.
 * Usage: vector<vll> d(n, vll(n, INF)); d[i][i] = 0;
 * d[u][v] = min(d[u][v], w); floydWarshall(d);
 * Time: O(N^3)
 * Status: stress-tested
 */
#pragma once

void floydWarshall(vector<vll>& d) {
	int n = sz(d);
	for (int k = 0; k < n; k++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				if (d[i][k] < INF && d[k][j] < INF)
					d[i][j] = min(d[i][j],
						max(d[i][k] + d[k][j], -INF));
	for (int k = 0; k < n; k++) if (d[k][k] < 0)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				if (d[i][k] < INF && d[k][j] < INF) d[i][j] = -INF;
}
