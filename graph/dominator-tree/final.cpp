#include <bits/stdc++.h>
using namespace std;

// 1-based, O(n + m), general graph
// call init() first, add edges, then call calc(source)
// result stored in I[v]
// I[v] = 0 if not reachable, else I[v] = idom of vertex v
// I[v] -> v forms a tree rooted at the source
const int SV = 202020;
namespace dominator_tree {
	int n, sp, dfn[SV], ord[SV], par[SV];
	int S[SV], I[SV];
	vector<int> gph[SV], rev[SV], V[SV];

	void init(int _n) { 
		n = _n; sp = 0;
		for (int i = 1; i <= n; i++) {
			dfn[i] = 0, ord[i] = 0, S[i] = 1e9, I[i] = 0;
			gph[i].clear();
			rev[i].clear();
		}
	}
	void add_edge(int u, int v) {
		gph[u].push_back(v);
		rev[v].push_back(u);
	}
	struct dsu {
		int par[SV], bst[SV];
		void init() {
			iota(par + 1, par + 1 + n, 1);
			iota(bst + 1, bst + 1 + n, 1);
		}
		void merge(int p, int v) { par[v] = p; }
		int calc(int v) {
			if (par[v] == v) return bst[v];
			int k = calc(par[v]);
			if (S[bst[v]] > S[k]) {
				bst[v] = k;
			}
			par[v] = par[par[v]];
			return bst[v];
		}
	} dsu;
	void dfs(int v) {
		dfn[v] = ++sp;
		ord[sp] = v;
		for (int x : gph[v]) if (!dfn[x]) {
			par[x] = v;
			dfs(x);
		}
	}
	void calc(int src) {
		dfs(src); dsu.init();
		I[src] = src;
		for (int i = n; i > 1; i--) if (ord[i] != 0) {
			int v = ord[i];
			for (int x : rev[v]) if (dfn[x]) {
				S[v] = min(S[v], dfn[x] > dfn[v] ? S[dsu.calc(x)] : dfn[x]);
			}
			dsu.merge(par[v], v);
			V[ord[S[v]]].push_back(v);
			for (int x : V[par[v]]) {
				int y = dsu.calc(x);
				I[x] = (S[x] == S[y]) ? ord[S[x]] : y;
			}
			V[par[v]].clear();
		}
		for (int i = 2; i <= n; i++) if (ord[i] != 0) {
			int v = ord[i];
			I[v] = (I[v] == ord[S[v]]) ? I[v] : I[I[v]];
		}
	}
};

// https://judge.yosupo.jp/problem/dominatortree
int main() {
	cin.tie(0); ios_base::sync_with_stdio(0);
	int n, m, s;
	cin >> n >> m >> s;
	dominator_tree::init(n);
	for (int i = 0; i < m; i++) {
		int u, v;
		cin >> u >> v;
		++u; ++v;
		dominator_tree::add_edge(u, v);
	}
	dominator_tree::calc(++s);
	for (int i = 1; i <= n; i++) cout << dominator_tree::I[i] - 1 << ' ';
	cout << '\n';
}