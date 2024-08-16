// 1-based, O(V + E), undirected graph
// call init() first, add_edge(u, v, i), then call get_bcc()
// bcnt : number of bcc
// bcc[c] : vertices of color c
// color[v] : colors of vertex v
// ecol[e] : color of edge e
// connect (v, c + N) for block-cut tree
const int SV = 5e5 + 10;
const int SE = 5e5 + 10;
int N, M;
vector<pii> adj[SV];
int dfn[SV], low[SV], dcnt;
bool vis[SV];
int bcnt, ecol[SE];
vector<int> color[SV], bcc[SE];
void init(int _N, int _M) {
  N = _N; M = _M;
  for (int i = 1; i <= N; i++) {
    dfn[i] = low[i] = vis[i] = 0;
    adj[i] = vector<pii>();
    color[i] = vector<int>();
  }
  for (int i = 1; i <= M; i++) {
    ecol[i] = 0;
    bcc[i] = vector<int>();
  }
  dcnt = 0; bcnt = 0;
}
void add_edge(int u, int v, int i) {
  // i is edge number of edge (u, v)
  adj[u].push_back({v, i});
  adj[v].push_back({u, i});
}
void dfs(int now, int bef) {
  dfn[now] = low[now] = ++dcnt;
  for (auto [nxt, p] : adj[now]) {
    if (nxt == bef) continue;
    if (dfn[nxt]) low[now] = min(low[now], dfn[nxt]);
    else {
      dfs(nxt, now);
      low[now] = min(low[now], low[nxt]);
    }
  }
}
void dfs2(int now, int col) {
  if (col) {
    color[now].push_back(col);
    bcc[col].push_back(now);
  }
  vis[now] = true;
  for (auto [nxt, p] : adj[now]) {
    if (dfn[nxt] < dfn[now]) ecol[p] = col;
    if (vis[nxt]) continue;
    if (low[nxt] >= dfn[now]) {
      bcnt++;
      ecol[p] = bcnt;
      color[now].push_back(bcnt);
      bcc[bcnt].push_back(now);
      dfs2(nxt, bcnt);
    } else {
      ecol[p] = col;
      dfs2(nxt, col);
    }
  }
}
void get_bcc() {
  for (int i = 1; i <= N; i++) if (!dfn[i]) dfs(i, i);
  for (int i = 1; i <= N; i++) if (!vis[i]) dfs2(i, 0);
  // Comment this if isolated vertices does not form a bcc
  for (int i = 1; i <= N; i++) if (adj[i].empty()) {
    bcnt++;
    color[i].push_back(bcnt);
    bcc[bcnt].push_back(i);
  }
}