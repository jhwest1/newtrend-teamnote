// 0-based
// call init() first, add_edge(u, v, l, r), then flow(src, snk)
// gets maximum flow for given LR-flow graph
// returns true if feasible, false if infeasible
// for circulation, call flow(-1, -1)
// for maximum LR flow, run mf.flow(src, snk) again
// for minimum LR flow, binary search with INF
struct Circulation {
  typedef long long T;
  const T INF = numeric_limits<T>::max() / 2;

  int n;
  T lbsum = 0;
  HLPP mf; // any max flow algorithm
  // dummy src = n, dummy snk = n+1
  void init(int k) {
    n = k;
    mf.init(n + 2);
    lbsum = 0;
  }
  void add_edge(int s, int e, T l, T r) {
    mf.add_edge(s, e, r - l);
    if (l > 0) {
      mf.add_edge(n, e, l);
      mf.add_edge(s, n + 1, l);
      lbsum += l;
    } else {
      mf.add_edge(n, s, -l);
      mf.add_edge(e, n + 1, -l);
      lbsum += -l;
    }
  }
  bool flow(int src, int snk) {
    // to reduce as maxflow with lower bounds, in circulation problem skip this line
    // mf.add_edge(snk, src, INF);
    return lbsum == mf.flow(n, n + 1);
    // to get maximum LR flow, run mf.flow(src, snk) again
  }
};

// 0-based, O(V^3 log(VC)), directed graph
// call init() first, add_edge(u, v, l, r, cost), then solve()
// gets minimum cost circulation in given graph
// returns {false, 0} if infeasible
// otherwise, returns {true, ans} 
// solution flow in edge (v, p) = gph[v][p].cap - gph[v][p].rem
struct MinCostCirculation {
  typedef long long T;
  const int SCALE = 3;  // scale by 1/(1 << SCALE)
  const T INF = numeric_limits<T>::max() / 2;

  struct EdgeStack {
    int s, e;
    T l, r, cost;
  };
  struct Edge {
    int pos, rev;
    T rem, cap, cost;
  };

  int n;
  vector<EdgeStack> estk;
  Circulation circ;
  vector<vector<Edge>> gph;
  vector<T> p;

  void init(int k) {
    n = k;
    circ.init(n);
    gph.clear(); gph.resize(n);
    p.clear(); p.resize(n);
  }
  void add_edge(int s, int e, T l, T r, T cost) {
    estk.push_back({s, e, l, r, cost});
  }
  pair<bool, T> solve() {
    for (auto &i : estk) if (i.s != i.e) circ.add_edge(i.s, i.e, i.l, i.r);
    if (!circ.flow(-1, -1)) return make_pair(false, T(0));

    vector<int> ptr(n);
    T eps = 0;
    for (auto &i : estk) {
      T curFlow;
      if (i.s != i.e) curFlow = i.r - circ.mf.gph[i.s][ptr[i.s]].c;
      else curFlow = i.r;
      int srev = gph[i.e].size();
      int erev = gph[i.s].size();
      if (i.s == i.e) srev++;
      gph[i.s].push_back({i.e, srev, i.r - curFlow, i.r, i.cost * (n + 1)});
      gph[i.e].push_back({i.s, erev, -i.l + curFlow, -i.l, -i.cost * (n + 1)});
      eps = max(eps, abs(i.cost) * (n + 1));
      if (i.s != i.e) {
        ptr[i.s] += 2;
        ptr[i.e] += 2;
      }
    }
    while (eps > 1) {
      eps = max(T(1), eps >> SCALE);
      auto cost = [&](Edge &e, int s, int t) { return e.cost + p[s] - p[t]; };
      vector<T> excess(n);
      queue<int> Q;
      auto push = [&](Edge &e, int src, T flow) {
        e.rem -= flow;
        gph[e.pos][e.rev].rem += flow;
        excess[src] -= flow;
        excess[e.pos] += flow;
        if (excess[e.pos] <= flow && excess[e.pos] > 0) Q.push(e.pos);
      };
      auto relabel = [&](int v) {
        p[v] = -INF;
        for (auto &e : gph[v]) {
          if (e.rem > 0) {
            p[v] = max(p[v], p[e.pos] - e.cost - eps);
          }
        }
      };
      for (int i = 0; i < n; i++) {
        for (auto &j : gph[i]) {
          if (j.rem > 0 && cost(j, i, j.pos) < 0) {
            push(j, i, j.rem);
          }
        }
      }
      while (!Q.empty()) {
        int x = Q.front();
        Q.pop();
        while (excess[x] > 0) {
          for (auto &e : gph[x]) {
            if (e.rem > 0 && cost(e, x, e.pos) < 0) {
              push(e, x, min(e.rem, excess[x]));
              if (excess[x] == 0) break;
            }
          }
          if (excess[x] == 0) break;
          relabel(x);
        }
      }
    }
    T ans = 0;
    for (int i = 0; i < n; i++) {
      for (auto &j : gph[i]) {
        j.cost /= (n + 1);
        ans += j.cost * (j.cap - j.rem);
      }
    }
    return make_pair(true, ans / 2);
  }
};