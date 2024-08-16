#include <bits/stdc++.h>
using namespace std;

// 1-based
// if left vertex x is not included in a matching, a[x] = -1.
// Otherwise a[x] stores the vertex x is matched with
// if right vertex x is not included in a matching, b[x] = -1. 
// Otherwise b[x] stores the vertex x is matched with
namespace hopcroft_karp {
	const int SZ = 101010;
	int n, m, a[SZ], b[SZ];
	vector<int> gph[SZ];

	void init(int _n, int _m) {
		n = _n;
		m = _m;
		for (int i = 1; i <= n; i++) gph[i].clear();
		fill(a, a + 1 + n, 0);
		fill(b, b + 1 + m, 0);
	}
	void add_edge(int u, int v) { 
		gph[u].push_back(v); // left vertex u -> right vertex v
	} 
	int ds[SZ], sp[SZ];
	void bfs() {
		fill(ds, ds + 1 + n, -1);
		queue<int> Q;
		for (int i = 1; i <= n; i++) {
			if (!a[i]) {
				ds[i] = 0;
				Q.push(i);
			}
		}
		while (!Q.empty()) {
			int v = Q.front(); Q.pop();
			for (int x : gph[v]) {
				if (b[x] && ds[b[x]] == -1) {
					ds[b[x]] = ds[v] + 1;
					Q.push(b[x]);
				}
			}
		}
	}
	bool match(int v) {
		for (; sp[v] < gph[v].size(); sp[v]++) {
			int x = gph[v][sp[v]];
			if (!b[x] || (ds[b[x]] == ds[v] + 1 && match(b[x]))) {
				b[x] = v; a[v] = x;
				return true;
			}
		}
		return false;
	}
	int find_matching() {
		int ret = 0;
		while (true) {
			fill(sp, sp + 1 + n, 0);
			bfs();
			int cur = 0;
			for (int i = 1; i <= n; i++) if (!a[i]) cur += match(i);
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
		hopcroft_karp::add_edge(u + 1, v + 1);
	}
	int ans = hopcroft_karp::find_matching();
	cout << ans << '\n';
	for (int i = 1; i <= n; i++) {
		if (hopcroft_karp::a[i]) cout << i - 1 << ' ' << hopcroft_karp::a[i] - 1 << '\n';
	}
}