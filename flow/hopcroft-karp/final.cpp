// 1-based, O(E \sqrt{V}), unweighted bipartite graph
// call init() first, then call find_matching()
// if left vertex x is not included in a matching, a[x] = 0.
// Otherwise a[x] stores the vertex x is matched with
// if right vertex x is not included in a matching, b[x] = 0.
// Otherwise b[x] stores the vertex x is matched with
const int SZ = 101010;
int n, m, a[SZ], b[SZ];
vector<int> gph[SZ];
void init(int _n, int _m) {
  n = _n;
  m = _m;
  for (int i = 1; i <= n; i++) gph[i].clear();
  fill(a, a + 1 + n, 0);
  fill(b, b + 1 + m, 0);
}
void add_edge(int u, int v) {
  gph[u].push_back(v);  // left vertex u -> right vertex v
}
int ds[SZ], sp[SZ];
void bfs() {
  fill(ds, ds + 1 + n, 0);
  queue<int> Q;
  for (int i = 1; i <= n; i++) {
    if (!a[i]) {
      ds[i] = 0;
      Q.push(i);
    }
  }
  while (!Q.empty()) {
    int v = Q.front();
    Q.pop();
    for (int x : gph[v]) {
      if (b[x] && ds[b[x]] == 0) {
        ds[b[x]] = ds[v] + 1;
        Q.push(b[x]);
      }
    }
  }
}
bool match(int v) {
  for (; sp[v] < gph[v].size(); sp[v]++) {
    int x = gph[v][sp[v]];
    if (!b[x] || (ds[b[x]] == ds[v] + 1 && match(b[x]))) {
      b[x] = v; a[v] = x;
      return true;
    }
  }
  return false;
}
int find_matching() {
  int ret = 0;
  while (true) {
    fill(sp, sp + 1 + n, 0);
    bfs();
    int cur = 0;
    for (int i = 1; i <= n; i++) if (!a[i]) cur += match(i);
    if (cur > 0) ret += cur;
    else break;
  }
  return ret;
}