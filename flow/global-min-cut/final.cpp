// 0-based, O(V^3), undirected graph
// call init() first, add_edge(u, v, w), then call solve()
// gets global min cut of given graph
// cut[v] = {0, 1} which side the vertex belongs to
const int SV = 505;
typedef int T;
const T INF = numeric_limits<T>::max();
int n;
T g[SV][SV];
vector<int> cut;
void init(int _n) {
  n = _n;
  for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) g[i][j] = 0;
}
void add_edge(int u, int v, T w) {
  if (u == v) return;
  g[u][v] += w;
  g[v][u] += w;
}
pair<T, pii> stMinCut(vector<int> &active) {
  vector<T> key(n);
  vector<int> v(n);
  int s = -1, t = -1;
  for (int i = 0; i < active.size(); i++) {
    T maxv = -1;
    int cur = -1;
    for (auto j : active) if (v[j] == 0 && maxv < key[j]) {
      maxv = key[j];
      cur = j;
    }
    t = s; s = cur;
    v[cur] = 1;
    for (auto j : active) key[j] += g[cur][j];
  }
  return {key[s], {s, t}};
}
T solve() {
  T res = INF;
  vector<vector<int>> grps;
  vector<int> active;
  cut.resize(n);
  for (int i = 0; i < n; i++) grps.push_back({1, i});
  for (int i = 0; i < n; i++) active.push_back(i);
  while (active.size() >= 2) {
    auto cur = stMinCut(active);
    if (cur.ff < res) {
      res = cur.ff;
      fill(cut.begin(), cut.end(), 0);
      for (auto v : grps[cur.ss.ff]) cut[v] = 1;
    }
    int s = cur.ss.ff, t = cur.ss.ss;
    if (grps[s].size() < grps[t].size()) swap(s, t);

    active.erase(find(active.begin(), active.end(), t));
    grps[s].insert(grps[s].end(), grps[t].begin(), grps[t].end());
    for (int i = 0; i < n; i++) { g[i][s] += g[i][t]; g[i][t] = 0; }
    for (int i = 0; i < n; i++) { g[s][i] += g[t][i]; g[t][i] = 0; }
    g[s][s] = 0;
  }
  return res;
}