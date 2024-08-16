#include <bits/stdc++.h>
#define ff first
#define ss second
using namespace std;

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

// 1-based
// MUST call init() before calling solve();
// vector<pii> par = solve();
// par represents 1-rooted tree. if i > 1, par[i] = (parent, cost)
// i-j cut in original graph = i-j cut in gomory-hu tree
// time complexity: O(V * T(maxflow))
namespace gomory_hu {
	const int SZ = 101010;
	typedef long long T;
	struct edge { 
		int u, v; T w; 
	};
	int n;
	vector<edge> edges;
	bool vis[SZ];
	Dinic mf;
	void init(int _n){ n = _n; edges.clear(); }
	void add_edge(int u, int v, T w){ edges.push_back({u, v, w}); }
	void dfs(int v){
		if (vis[v]) return;
		vis[v] = true;
		for (auto &e : mf.gph[v]) if (e.c > 0) dfs(e.v);
	}
	vector<pair<int, T>> solve(){
		vector<pair<int, T>> ret(n + 1, {1, 0});
		for(int i = 2; i <= n; i++){
			mf.init(n + 1);
			for(auto [u, v, w] : edges){
				mf.add_edge(u, v, w, false);
			}
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
};

// manual test on a random graph
mt19937 rnd(1557);
int rng(int l, int r) {
	return uniform_int_distribution<int>(l, r)(rnd);
}
typedef pair<int, int> pii;
vector<pii> g[2020];
int dep[2020], par[2020], pc[2020];
void dfs(int v) {
	for (auto [w, x] : g[v]) {
		dep[x] = dep[v] + 1;
		par[x] = v;
		pc[x] = w;
		dfs(x);
	}
}
int main() {
	int n = 150, m = 2000;
	int u[m], v[m], w[m];
	for (int i = 0; i < m; i++) {
		u[i] = rng(1, n);
		v[i] = rng(1, n);
		while (u[i] == v[i]) {
			v[i] = rng(1, n);
		}
		w[i] = rng(1, 1000);
	}
	gomory_hu::init(n);
	for (int i = 0; i < m; i++) {
		gomory_hu::add_edge(u[i], v[i], w[i]);
	}
	auto ret = gomory_hu::solve();
	for (int i = 2; i <= n; i++) {
		g[ret[i].ff].push_back({ret[i].ss, i});
	}
	dfs(1);
	for (int U = 1; U <= n; U++) {
		for (int V = U + 1; V <= n; V++) {
			Dinic dinic;
			dinic.init(n + 1);
			for (int i = 0; i < m; i++) {
				dinic.add_edge(u[i], v[i], w[i], false);
			}
			int ans = dinic.flow(U, V);
			// cout << "between " << U << " and " << V << " -> ";
			// cout << ans << ' ';
			int x = U, y = V;
			if (dep[x] > dep[y]) swap(x, y);
			int ret = 1e9;
			while (dep[y] != dep[x]) {
				ret = min(ret, pc[y]);
				y = par[y];
			}
			while (x != y) {
				ret = min(ret, min(pc[x], pc[y]));
				x = par[x];
				y = par[y];
			}
			// cout << ret << '\n';
			assert(ans == ret);
		}
	}
}