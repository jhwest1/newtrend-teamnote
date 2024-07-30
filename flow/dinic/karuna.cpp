#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// 0-based
namespace dinic {
	typedef long long T;
	const T INF = (1ll << 63) - 1;
	const int SZ = 101010;
	struct edge {
		int v; T c; int d;
	};
	vector<edge> g[SZ];
	int n, src, snk;
	
	void init(int _n) {
		for (int i = 0; i < n; i++) g[i].clear();
		n = _n;
	}
	void add_edge(int u, int v, T c, bool dir = true) {
		g[u].push_back({v, c, (int)g[v].size()});
		g[v].push_back({u, dir ? 0 : c, (int)g[u].size() - 1});
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
	T dfs(int v, T f) {
		if (v == snk) return f;
		for (; sp[v] < g[v].size(); sp[v]++) {
			auto &e = g[v][sp[v]];
			if (e.c == 0 || ds[e.v] != ds[v] + 1) continue;

			T h = dfs(e.v, min(f, e.c));
			if (h > 0) {
				e.c -= h;
				g[e.v][e.d].c += h;
				return h;
			}
		}
		return 0;
	}
	
	T flow(int _src, int _snk) {
		src = _src; snk = _snk;
		T ret = 0;
		while (true) {
			for (int i = 0; i < n; i++) sp[i] = 0;
			bfs();

			T cur = 0;
			while (true) {
				int f = dfs(src, INF);
				if (f > 0) cur += f;
				else break;
			}
			if (cur > 0) ret += cur;
			else break;
		}
		return ret;
	}
};

// https://loj.ac/p/127
int main() {
	cin.tie(0); ios_base::sync_with_stdio(0);
	int n, m, s, t;
	cin >> n >> m >> s >> t;
	dinic::init(n);
	for (int i = 0; i < m; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		dinic::add_edge(u - 1, v - 1, w);
	}
	cout << dinic::flow(s - 1, t - 1) << '\n';
}