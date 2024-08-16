#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// 1-based, O(m sqrt m)
// at most O(m sqrt m) triangles, achieved when K_n
// at most O(m^2) 4-cycles, achieved when K_n
void solve(int x, int y, int z);
namespace count_3_4_cyc {
	const int SV = 101010;
	int n, cnt[SV], ord[SV], rnk[SV];
	vector<int> gph[SV];
	void init(int _n) { 
		n = _n;
		for (int i = 1; i <= n; i++) gph[i].clear();
	}
	void add_edge(int u, int v) {
		gph[u].push_back(v);
		gph[v].push_back(u);
	}
	void precalc() {
		iota(ord + 1, ord + 1 + n, 0);
		sort(ord + 1, ord + 1 + n, [&](int i, int j) { return gph[i].size() < gph[j].size(); });
		for (int i = 1; i <= n; i++) rnk[ord[i]] = i;
	}
	void enumerate_triangles() {
		precalc();
		for (int i = 1; i <= n; i++) {
			int x = ord[i];
			for (int y : gph[x]) cnt[y] = true;
			for (int y : gph[x]) if (rnk[y] < rnk[x]) {
				for (int z : gph[y]) if (cnt[z] && rnk[z] < rnk[y]) {
					// x, y, z forms a triangle
					solve(x, y, z);
				}
			}
			for (int y : gph[x]) cnt[y] = false;
		}
	}
	ll count_4_cycles() {
		precalc();
		ll ret = 0;
		for (int i = 1; i <= n; i++) {
			int x = ord[i];
			for (int y : gph[x]) if (rnk[y] < rnk[x]) {
				for (int z : gph[y]) if (rnk[z] < rnk[y]) {
					ret += cnt[z]++;
				}
			}
			for (int y : gph[x]) if (rnk[y] < rnk[x]) for (int z : gph[y]) cnt[z] = 0;
		}
		return ret;
	}
}

// https://judge.yosupo.jp/problem/enumerate_triangles
const int MOD = 998244353;
ll a[101010], ans = 0;
void solve(int x, int y, int z) {
	ans = (ans + a[x] * a[y] % MOD * a[z] % MOD) % MOD;
}
int main() {
	cin.tie(0); ios_base::sync_with_stdio(0);
	int n, m;
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	count_3_4_cyc::init(n);
	for (int i = 0; i < m; i++) {
		int u, v;
		cin >> u >> v;
		count_3_4_cyc::add_edge(u + 1, v + 1);
	}
	count_3_4_cyc::enumerate_triangles();
	cout << ans << '\n';
}