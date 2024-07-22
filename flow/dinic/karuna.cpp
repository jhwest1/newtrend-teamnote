#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

namespace dinic {
	const int SZ = 101010;
	struct edge {
		int v, c, d;
	};
	vector<edge> g[SZ];
	int n, src, snk;
	
	void init(int _n, int _src, int _snk) {
		for (int i = 0; i < n; i++) g[i].clear();
		n = _n;
		src = _src;
		snk = _snk;
	}
	void add_edge(int u, int v, int c) {
		g[u].push_back({v, c, (int)g[v].size()});
		g[v].push_back({u, 0, (int)g[u].size() - 1});
	}
	int sp[SZ], ds[SZ];
	void bfs() {
		for (int i = 0; i < n; i++) ds[i] = -1;
		queue<int> Q;
		ds[src] = 0;
		Q.push(src);
		while (!Q.empty()) {
			int v = Q.front(); Q.pop();
			for (auto &e : g[v]) {
				if (e.c > 0 && ds[e.v] == -1) {
					ds[e.v] = ds[v] + 1;
					Q.push(e.v);
				}
			}
		}
	}
	int dfs(int v, int f) {
		if (v == snk) return f;
		for (; sp[v] < g[v].size(); sp[v]++) {
			auto &e = g[v][sp[v]];
			if (e.c == 0 || ds[e.v] != ds[v] + 1) continue;

			int h = dfs(e.v, min(f, e.c));
			if (h > 0) {
				e.c -= h;
				g[e.v][e.d].c += h;
				return h;
			}
		}
		return 0;
	}
	
	ll flow() {
		ll ret = 0;
		while (true) {
			for (int i = 0; i < n; i++) sp[i] = 0;
			bfs();

			ll cur = 0;
			while (true) {
				int f = dfs(src, (1 << 31) - 1);
				if (f > 0) cur += f;
				else break;
			}
			if (cur > 0) ret += cur;
			else break;
		}
		return ret;
	}
};

// LOJ 127
int main() {
	cin.tie(0); ios_base::sync_with_stdio(0);
	int n, m, s, t;
	cin >> n >> m >> s >> t;
	dinic::init(n, s - 1, t - 1);
	for (int i = 0; i < m; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		dinic::add_edge(u - 1, v - 1, w);
	}
	cout << dinic::flow() << '\n';
}