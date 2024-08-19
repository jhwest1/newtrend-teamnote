// Include any max flow algorithm here

// 1-based
// MUST call init() before calling solve();
// vector<pii> par = solve();
// par represents 1-rooted tree. if i > 1, par[i] = (parent, cost)
// i-j cut in original graph = i-j cut in gomory-hu tree
// time complexity: O(V * T(maxflow))
const int SZ = 101010;
typedef long long T;
struct edge { int u, v; T w; };
int n;
vector<edge> edges;
bool vis[SZ];
Dinic mf;
void init(int _n) { n = _n; edges.clear(); }
void add_edge(int u, int v, T w) { edges.push_back({u, v, w}); }
void dfs(int v) {
  if (vis[v]) return;
  vis[v] = true;
  for (auto &e : mf.gph[v]) if (e.c > 0) dfs(e.v);
}
vector<pair<int, T>> solve() {
  vector<pair<int, T>> ret(n + 1, {1, 0});
  for (int i = 2; i <= n; i++) {
    mf.init(n + 1);
    for (auto [u, v, w] : edges) mf.add_edge(u, v, w, false);
    ret[i].ss = mf.flow(i, ret[i].ff);
    for (int j = 1; j <= n; j++) vis[j] = 0;
    dfs(i);
    for (int j = i + 1; j <= n; j++) {
      if (ret[j].ff == ret[i].ff && vis[j]) {
        ret[j].ff = i;
      }
    }
  }
  return ret;
}