// 0-based, O(EF), directed graph
// call init() first, add_edge(u, v, c(capcity), w(cost)), then flow(source, sink)
// returns (max flow, min cost when max flow)
struct MCMF {
  typedef long long T;
  const static int SV = 400;
  const T INF = numeric_limits<T>::max();

  struct Edge { int v; T c, w; int r; };
  int N, src, snk;
  vector<Edge> gph[SV];
  T dist[SV];
  int par[SV];
  bool inq[SV];

  void init(int _N) {
    N = _N;
    for (int i = 0; i < N; i++) gph[i].clear();
  }
  void add_edge(int u, int v, T c, T w) {
    gph[u].push_back({v, c, w, gph[v].size()});
    gph[v].push_back({u, 0, -w, gph[u].size() - 1});
  }
  bool shortest_path() {
    fill(dist, dist + N, INF);
    fill(par, par + N, -1);
    fill(inq, inq + N, false);
    deque<int> Q;
    dist[src] = 0;
    inq[src] = true;
    Q.push_back(src);
    int cnt = 0;
    while (Q.size()) {
      int x = Q.front();
      Q.pop_front();
      inq[x] = false;
      for (auto [y, c, w, r] : gph[x]) {
        if (c == 0) continue;
        if (dist[y] > dist[x] + w) {
          dist[y] = max(dist[x] + w, -INF);
          par[y] = r;
          if (!inq[y]) {
            inq[y] = true;
            if (Q.size() && dist[Q[0]] > dist[y]) Q.push_front(y);
            else Q.push_back(y);
          }
        }
      }
    }
    return dist[snk] != INF;
  }
  pair<T, T> flow(int _src, int _snk) {
    src = _src;
    snk = _snk;
    T cost = 0, flow = 0;
    while (shortest_path()) {
      T val = INF;
      for (int x = snk; x != src;) {
        auto [y, c, w, r] = gph[x][par[x]];
        val = min(val, gph[y][r].c);
        x = y;
      }
      for (int x = snk; x != src;) {
        auto [y, c, w, r] = gph[x][par[x]];
        gph[y][r].c -= val;
        gph[x][par[x]].c += val;
        x = y;
      }
      cost += dist[snk] * val;
      flow += val;
    }
    return {flow, cost};
  }
};