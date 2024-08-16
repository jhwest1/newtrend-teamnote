// 0-based, min( O(V^2 E), O(EF) ), directed graph
// call init() first, add_edge(u, v, c), then flow(source, sink)
struct Dinic {
  typedef long long T;
  const static int SV = 1200;
  const T INF = numeric_limits<T>::max();

  struct Edge { int v; T c; int r; };
  int N, src, snk;
  vector<Edge> gph[SV];
  int lvl[SV], pos[SV];
  T lim;
  void init(int _N) {
    N = _N;
    for (int i = 0; i < N; i++) gph[i] = vector<Edge>();
  }
  void add_edge(int u, int v, T c, bool dir = true) {
    // directed edge : dir = true, undirected edge : dir = false
    gph[u].push_back({v, c, gph[v].size()});
    gph[v].push_back({u, dir ? 0 : c, gph[u].size() - 1});
  }
  bool bfs() {
    for (int i = 0; i < N; i++) lvl[i] = 0;
    queue<int> Q;
    Q.push(src);
    lvl[src] = 1;
    while (!Q.empty()) {
      int now = Q.front(); Q.pop();
      for (auto [nxt, c, r] : gph[now]) {
        if (lvl[nxt] || c < lim) continue;
        Q.push(nxt);
        lvl[nxt] = lvl[now] + 1;
      }
    }
    return lvl[snk];
  }
  T dfs(int now, T flow) {
    if (now == snk) return flow;
    for (; pos[now] < gph[now].size(); pos[now]++) {
      auto &[nxt, c, r] = gph[now][pos[now]];
      if (lvl[nxt] != lvl[now] + 1 || !c) continue;
      T f = dfs(nxt, min(flow, c));
      if (f) {
        c -= f;
        gph[nxt][r].c += f;
        return f;
      }
    }
    return 0;
  }
  T flow(int _src, int _snk) {
    src = _src; snk = _snk;
    T ans = 0;
    for (lim = INF; lim > 0; lim >>= 1) {
      while (bfs()) {
        for (int i = 0; i < N; i++) pos[i] = 0;
        while (1) {
          T t = dfs(src, INF);
          if (!t) break;
          ans += t;
        }
      }
    }
    return ans;
  }
};