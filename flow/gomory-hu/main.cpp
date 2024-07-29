#include <bits/stdc++.h>
#define ff first
#define ss second
using namespace std;

/* include flow/dinic/karuna.cpp here */


// 1-based
// MUST call init() before calling solve();
// vector<pii> par = solve();
// par represents 1-rooted tree. if i > 1, par[i] = (parent, cost)
// i-j cut in original graph = i-j cut in gomory-hu tree
// time complexity: O(V * T(maxflow))
const int SZ = 101010;
namespace gomory_hu {
	typedef long long T;
	struct edge { 
		int u, v; T w; 
	};
	vector<edge> edges;
	int n;
	void init(int _n){ n = _n; edges.clear(); }
	void add_edge(int u, int v, T w){ edges.push_back({u, v, w}); }
	bool vis[SZ];
	void dfs(int v){
		if (vis[v]) return;
		vis[v] = true;
		for (auto &e : dinic::g[v]) if (e.c > 0) dfs(e.v);
	}
	vector<pair<int, T>> solve(){
		vector<pair<int, T>> ret(n + 1, {1, 0});
		for(int i = 2; i <= n; i++){
			dinic::init(n + 1);
			for(auto [u, v, w] : edges){
				dinic::add_edge(u, v, w, false);
			}
			ret[i].ss = dinic::flow(i, ret[i].ff);
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
			dinic::init(n + 1);
			for (int i = 0; i < m; i++) {
				dinic::add_edge(u[i], v[i], w[i], false);
			}
			// cout << "between " << U << " and " << V << " -> ";
			// cout << dinic::flow(U, V) << ' ';
			int ans = dinic::flow(U, V);
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