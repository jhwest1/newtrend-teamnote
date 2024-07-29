#include <bits/stdc++.h>
using namespace std;

const int SV = 2020;
namespace blossom {
	int n, t;
	int match[SV], par[SV], vis[SV], org[SV], aux[SV];
	vector<int> g[SV], S;

	void init(int _n) {
		n = _n; t = 0;
	}
	void add_edge(int u, int v) {
		g[u].push_back(v);
		g[v].push_back(u);
	}
	int lca(int x, int y) {
		++t;
		while (true) {
			if (x) {
				if (aux[x] == t) return x;
				aux[x] = t;
				x = org[par[match[x]]];
			}
			swap(x, y);
		}
	}
	void blossom(int x, int y, int z) {
		while (org[x] != z) {
			par[x] = y; y = match[x];
			if (vis[y] == 1) {
				vis[y] = 0;
				S.push_back(y);
			}
			org[x] = org[y] = z;
			x = par[y];
		}
	}
	void augment(int x) {
		while (x) {
			int px = par[x];
			int nx = match[px];
			match[x] = px;
			match[px] = x;
			x = nx;
		}
	}
	bool bfs(int s) {
		for (int i = 1; i <= n; i++) vis[i] = -1, org[i] = i;
		S.clear();
		S.push_back(s);
		vis[s] = 0;
		while (!S.empty()) {
			int v = S.back();
			S.pop_back();
			for (int x : g[v]) {
				if (vis[x] == -1) {
					vis[x] = 1; par[x] = v;
					if (!match[x]) {
						augment(x); return true;
					}
					vis[match[x]] = 0;
					S.push_back(match[x]);
				}
				else if (vis[x] == 0 && org[x] != org[v]) {
					int z = lca(org[x], org[v]);
					blossom(v, x, z);
					blossom(x, v, z);
				}
			}
		}
		return false;
	}
	int general_matching() {
		int cnt = 0;
		for (int v = 1; v <= n; v++) {
			if (!match[v]) {
				cnt += bfs(v);
			}
		}
		return cnt;
	}
};

// https://judge.yosupo.jp/problem/general_matching
int main() {
	cin.tie(0); ios_base::sync_with_stdio(0);
	int n, m;
	cin >> n >> m;
	blossom::init(n);
	for (int i = 0; i < m; i++) {
		int u, v;
		cin >> u >> v;
		blossom::add_edge(++u, ++v);
	}
	int ret = blossom::general_matching();
	cout << ret << '\n';
	for (int i = 1; i <= n; i++) {
		if (blossom::match[i] > i) cout << i - 1 << ' ' << blossom::match[i] - 1 << '\n';
	}
}