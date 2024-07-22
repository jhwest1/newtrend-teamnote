#include <bits/stdc++.h>
using namespace std;

// 0-based
// if left vertex x is not included in a matching, a[x] = -1. Otherwise a[x] stores the vertex x is matched with
// if right vertex x is not included in a matching, b[x] = -1. Otherwise b[x] stores the vertex x is matched with
namespace hopcroft_karp {
	const int SZ = 101010;

	int n, m, a[SZ], b[SZ];
	vector<int> g[SZ];

	void init(int _n, int _m) {
		n = _n;
		m = _m;
		for (int i = 0; i < n; i++) a[i] = -1;
		for (int i = 0; i < m; i++) b[i] = -1;
	}
	void add_edge(int u, int v) { g[u].push_back(v); } // left vertex u -> right vertex v

	int ds[SZ], sp[SZ];
	void bfs() {
		queue<int> Q;
		for (int i = 0; i < n; i++) {
			if (a[i] == -1) {
				ds[i] = 0;
				Q.push(i);
			}
			else ds[i] = -1;
		}
		while (!Q.empty()) {
			int v = Q.front(); Q.pop();
			for (int x : g[v]) {
				if (b[x] != -1 && ds[b[x]] == -1) {
					ds[b[x]] = ds[v] + 1;
					Q.push(b[x]);
				}
			}
		}
	}
	bool match(int v) {
		while (sp[v] < g[v].size()) {
			int x = g[v][sp[v]++];
			if (b[x] == -1 || (ds[b[x]] == ds[v] + 1 && match(b[x]))) {
				b[x] = v; a[v] = x;
				return true;
			}
		}
		return false;
	}
	int find_matching() {
		int ret = 0;
		while (true) {
			for (int i = 0; i < n; i++) sp[i] = 0;
			bfs();
			int cur = 0;
			for (int i = 0; i < n; i++) if (a[i] == -1) cur += match(i);
			if (cur > 0) ret += cur;
			else break;
		}
		return ret;
	}
};

// https://judge.yosupo.jp/problem/bipartitematching
int main() {
	cin.tie(0); ios_base::sync_with_stdio(0);
	int n, m, k;
	cin >> n >> m >> k;
	hopcroft_karp::init(n, m);
	for (int i = 0; i < k; i++) {
		int u, v; 
		cin >> u >> v;
		hopcroft_karp::add_edge(u, v);
	}
	int ans = hopcroft_karp::find_matching();
	cout << ans << '\n';
	for (int i = 0; i < n; i++) {
		if (hopcroft_karp::a[i] != -1) cout << i << ' ' << hopcroft_karp::a[i] << '\n';
	}
}