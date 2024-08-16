// 1-based, O(V^3), weighted bipartite graph
// solves min cost matching of a perfect bipartite graph
// call init() first, add_edge(u, v, w), then call min_cost_matching()
// edges can be negative -> for max cost just negate all costs then negate the answer
// multiple edges are NOT allowed
// left vertex v is matched with right vertex a[v], a[v] = 0 if not matched
// right vertex v is matched with left vertex b[v], b[v] = 0 if not matched
typedef long long T;
const int SZ = 505;
int n, a[SZ], b[SZ], m[SZ], p[SZ], vis[SZ];
T w[SZ][SZ], x[SZ], y[SZ];
void init(int _n) {
  n = _n;
  // it must hold that x[i] + y[j] <= w[i][j] initially
  for (int i = 1; i <= n; i++) {
    x[i] = y[i] = -1e9;
    a[i] = b[i] = 0;
    for (int j = 1; j <= n; j++) w[i][j] = 0;
  }
}
void add_edge(int u, int v, T c) { w[u][v] = c; }
T f(int i, int j) { return w[i][j] - x[i] - y[j]; }
int dfs(int v) {
  vis[v] = 1;
  for (int i = 1; i <= n; i++) if (m[i] == 0 || f(m[i], i) > f(v, i)) m[i] = v;
  for (int i = 1; i <= n; i++) if (f(v, i) == 0 && !p[i]) {
    p[i] = v;
    if (int r = b[i] ? dfs(b[i]) : i; r) return r;
  }
  return 0;
}
void go(int v) {
  for (int i = 1; i <= n; i++) m[i] = p[i] = vis[i] = 0;
  int r = dfs(v);
  while (!r) {
    for (int i = 1; i <= n; i++) if (!p[i]) {
      if (r == 0 || f(m[r], r) > f(m[i], i)) r = i;
    }
    T a = f(m[r], r);
    for (int i = 1; i <= n; i++) {
      if (vis[i]) x[i] += a;
      if (p[i]) y[i] -= a;
    }
    p[r] = m[r];
    r = b[r] ? dfs(b[r]) : r;
  }
  while (r) swap(r, a[b[r] = p[r]]);
}
T min_cost_matching() {
  T ret = 0;
  for (int i = 1; i <= n; i++) go(i);
  for (int i = 1; i <= n; i++) ret += x[i] + y[i];
  return ret;
}