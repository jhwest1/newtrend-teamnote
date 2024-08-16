// 1-based, O(E sqrt E), undirected graph
// at most O(E sqrt E) triangles, achieved when K_n
// at most O(E^2) 4-cycles, achieved when K_n
// call init() first, add_edge(u, v), then call enumerate_triangles() or count_4_cycles()
void solve(int x, int y, int z);
const int SV = 101010;
int n, cnt[SV], ord[SV], rnk[SV];
vector<int> gph[SV];
void init(int _n) {
  n = _n;
  for (int i = 1; i <= n; i++) gph[i].clear();
}
void add_edge(int u, int v) {
  gph[u].push_back(v);
  gph[v].push_back(u);
}
void precalc() {
  iota(ord + 1, ord + 1 + n, 0);
  sort(ord + 1, ord + 1 + n, [&](int i, int j) { return gph[i].size() < gph[j].size(); });
  for (int i = 1; i <= n; i++) rnk[ord[i]] = i;
}
void enumerate_triangles() {
  precalc();
  for (int i = 1; i <= n; i++) {
    int x = ord[i];
    for (int y : gph[x]) cnt[y] = true;
    for (int y : gph[x]) if (rnk[y] < rnk[x]) {
      for (int z : gph[y]) if (cnt[z] && rnk[z] < rnk[y]) {
        // x, y, z forms a triangle
        solve(x, y, z);
      }
    }
    for (int y : gph[x]) cnt[y] = false;
  }
}
ll count_4_cycles() {
  precalc();
  ll ret = 0;
  for (int i = 1; i <= n; i++) {
    int x = ord[i];
    for (int y : gph[x]) if (rnk[y] < rnk[x]) {
      for (int z : gph[y]) if (rnk[z] < rnk[y]) {
        ret += cnt[z]++;
      }
    }
    for (int y : gph[x]) if (rnk[y] < rnk[x]) for (int z : gph[y]) cnt[z] = 0;
  }
  return ret;
}