#include <bits/stdc++.h>
using namespace std;

// 1-based, O(n log n)
// call hash_init() exactly ONCE before using
// then call tree_init(int n) per each tree before adding any edges
// use unrooted_hash() if unrooted
// use rooted_hash(parent, vertex) for unrooted/subtree hash
typedef unsigned long long ull;
namespace tree_isomorphism {
	const int SZ = 1010101;
	int n, sub[SZ], par[SZ], dep[SZ];
	vector<int> gph[SZ];

	mt19937 rnd(1557);
	ull rng() { return uniform_int_distribution<ull>(0, -1)(rnd); }

	ull A[SZ], B[SZ];
	void hash_init() {
		for (int i = 0; i < SZ; i++) {
			A[i] = rng();
			B[i] = rng();
		}
	}
	void tree_init(int _n) {
		n = _n;
		for (int i = 1; i <= n; i++) gph[i].clear();
	}
	void add_edge(int u, int v) {
		gph[u].push_back(v);
		gph[v].push_back(u);
	}
	void dfs(int p, int v) {
		sub[v] = 1;
		for (int x : gph[v]) if (p != x) {
			par[x] = v;
			dfs(v, x);
			sub[v] += sub[x];
		}
	}
	int centroid(int p, int v, int tot) {
		for (int x : gph[v]) if (p != x) {
			if (sub[x] * 2 >= tot) return centroid(v, x, tot);
		}
		return v;
	}
	ull rooted_hash(int p, int v) {
		vector<ull> V = {1};
		dep[v] = 0;
		for (int x : gph[v]) if (p != x) {
			ull r = rooted_hash(v, x);
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
	ull unrooted_hash() {
		dfs(-1, 1);
		int x = centroid(-1, 1, sub[1]);
		if (2 * sub[x] == sub[1]) return rooted_hash(x, par[x]) * rooted_hash(par[x], x);
		else return rooted_hash(-1, x);
	}
};

// https://www.acmicpc.net/problem/18123
int main() {
	cin.tie(0); ios_base::sync_with_stdio(0);
	int q;
	cin >> q;
	tree_isomorphism::hash_init();
	set<ull> ST;
	for (int i = 0; i < q; i++) {
		int n;
		cin >> n;
		tree_isomorphism::tree_init(n);
		for (int j = 0; j < n - 1; j++) {
			int u, v;
			cin >> u >> v;
			tree_isomorphism::add_edge(u + 1, v + 1);
		}
		ull ret = tree_isomorphism::unrooted_hash();
		ST.insert(ret);
	}
	cout << ST.size() << '\n';
}