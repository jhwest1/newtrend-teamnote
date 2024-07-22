#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long ull;
const int SZ = 1010101;

// 0-based, unrooted hash
namespace tree_isomorphism {
	int n, sub[SZ], par[SZ], dep[SZ];
	vector<int> g[SZ];

	mt19937 rnd(1557);
	ull rng() { return uniform_int_distribution<ull>(0, -1)(rnd); }

	ull A[SZ], B[SZ];
	void init() {
		for (int i = 0; i < SZ; i++) {
			A[i] = rng();
			B[i] = rng();
		}
	}
	void clear() {
		for (int i = 0; i < n; i++) {
			g[i].clear();
			dep[i] = 0;
		}
	}
	void add_edge(int u, int v) {
		g[u].push_back(v);
		g[v].push_back(u);
	}
	void dfs(int p, int v) {
		sub[v] = 1;
		for (int x : g[v]) if (p != x) {
			par[x] = v;
			dfs(v, x);
			sub[v] += sub[x];
		}
	}
	int centroid(int p, int v, int tot) {
		for (int x : g[v]) if (p != x) {
			if (sub[x] * 2 >= tot) return centroid(v, x, tot);
		}
		return v;
	}
	ull subtree_hash(int p, int v) {
		vector<ull> V = {1};
		for (int x : g[v]) if (p != x) {
			ull r = subtree_hash(v, x);
			dep[v] = max(dep[v], dep[x] + 1);
			V.push_back(r);
		}
		sort(V.begin(), V.end());
		ull ret = 0;
		for (int i = 0; i < V.size(); i++) {
			ret += V[i] ^ A[dep[v]] ^ B[i];
		}
		return ret;
	}
	ull get_hash() {
		dfs(-1, 0);
		int x = centroid(-1, 0, sub[0]);

		if (2 * sub[x] == sub[0]) return subtree_hash(x, par[x]) * subtree_hash(par[x], x);
		else return subtree_hash(-1, x);
	}
};

// https://www.acmicpc.net/problem/18123
int main() {
	cin.tie(0); ios_base::sync_with_stdio(0);
	int q;
	cin >> q;
	tree_isomorphism::init();
	set<ull> ST;
	for (int i = 0; i < q; i++) {
		int n;
		cin >> n;
		tree_isomorphism::n = n;
		for (int j = 0; j < n - 1; j++) {
			int u, v;
			cin >> u >> v;
			tree_isomorphism::add_edge(u, v);
		}
		ull ret = tree_isomorphism::get_hash();
		ST.insert(ret);
		tree_isomorphism::clear();
	}
	cout << ST.size() << '\n';
}