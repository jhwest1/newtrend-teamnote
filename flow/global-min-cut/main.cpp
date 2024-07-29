#include <bits/stdc++.h>
#define ff first
#define ss second
using namespace std;

typedef pair<int, int> pii;

// 0-based, O(V^3), undirected graph
// call init() before calling solve()
// cut[v] = {0, 1} which side the vertex belongs to 
namespace global_mincut {
	const int SV = 505;
    typedef int T;
	const T INF = (1 << 31) - 1;
    int n; T g[SV][SV];
    void init(int _n) {
        n = _n;
		for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) g[i][j] = 0;
    }
    void add_edge(int u, int v, T w) {
        if (u == v) return;
        g[u][v] += w; g[v][u] += w;
    }
    pair<T, pii> stMinCut(vector<int> &active) {
        vector<T> key(n);
        vector<int> v(n);
        int s = -1, t = -1;
        for (int i = 0; i < active.size(); i++) {
            T maxv = -1;
            int cur = -1;
            for (auto j : active) {
                if (v[j] == 0 && maxv < key[j]) {
                    maxv = key[j];
                    cur = j;
                }
            }
            t = s; s = cur;
            v[cur] = 1;
            for (auto j : active) key[j] += g[cur][j];
        }
        return {key[s], {s, t}};
    }
    vector<int> cut;
    T solve() {
        T res = INF;
        vector<vector<int>> grps;
        vector<int> active;
        cut.resize(n);
        for (int i = 0; i < n; i++) grps.push_back({1, i});
        for (int i = 0; i < n; i++) active.push_back(i);
        while (active.size() >= 2) {
            auto cur = stMinCut(active);
            if (cur.ff < res) {
                res = cur.ff;
                fill(cut.begin(), cut.end(), 0);
                for (auto v : grps[cur.ss.ff]) cut[v] = 1;
            }
            int s = cur.ss.ff, t = cur.ss.ss;
            if (grps[s].size() < grps[t].size()) swap(s, t);

            active.erase(find(active.begin(), active.end(), t));
            grps[s].insert(grps[s].end(), grps[t].begin(), grps[t].end());
            for (int i = 0; i < n; i++) { g[i][s] += g[i][t]; g[i][t] = 0; }
            for (int i = 0; i < n; i++) { g[s][i] += g[t][i]; g[t][i] = 0; }
            g[s][s] = 0;
        }
        return res;
    }
};

// manual testing for random graphs

/* for testing, include flow/dinic/karuna.cpp and flow/gomory-hu/main.cpp here */

mt19937 rnd(1557);
int rng(int l, int r) {
	return uniform_int_distribution<int>(l, r)(rnd);
}
int main() {
	int n = 5, m = 4000;
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
	for (int i = 0; i < m; i++) gomory_hu::add_edge(u[i], v[i], w[i]);
	global_mincut::init(n);
	for (int i = 0; i < m; i++) global_mincut::add_edge(u[i] - 1, v[i] - 1, w[i]);
	int ret = global_mincut::solve();
	auto V = gomory_hu::solve();
	long long ans = 1e9;
	for (int i = 2; i <= n; i++) ans = min(ans, V[i].ss);
	assert(ans == ret);
	cout << ans << ' ' << ret << '\n';
}