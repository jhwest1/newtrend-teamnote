#include <bits/stdc++.h>
#define pii pair<int, int>
#define pll pair<long long, long long>
#define piii pair<int, pii>
#define plll pair<long long, pll>
#define tiii array<int, 3>
#define tiiii array<int, 4>
#define ff first
#define ss second
#define ee ss.ff
#define rr ss.ss
typedef long long ll;
typedef long double ld;
using namespace std;

// call dmst(n, r, E); where E is the edge list of the graph
// return the cost of r-rooted mst, and parent of each node (r itself for parent of r)
// all nodes should be reachable from r. dmst() doesn't check about it :<
// ALERT: NODES ARE 0-BASED!!!!!!!!
typedef long long T;
typedef pair<T, T> ptt;
struct UF1 {
	int n;
	vector<int> par;
	UF1(int n) : n(n), par(n) { iota(par.begin(), par.end(), 0); }
	int fnd(int x) { return par[x] == x ? x : par[x] = fnd(par[x]); }
	bool uni(int x, int y) { x = fnd(x), y = fnd(y); if(x == y) return false; par[x] = y; return true; }
};
struct UF2 {
	int n;
	vector<int> par;
	vector<pii> his;
	UF2(int n) : n(n), par(n), his() { iota(par.begin(), par.end(), 0); }
	int fnd(int x) { return par[x] == x ? x : (his.push_back({x, par[x]}), par[x] = fnd(par[x])); }
	void uni(int x, int y) { x = fnd(x), y = fnd(y); his.push_back({x, par[x]}); par[x] = y; }
};
struct Edge{int u, v; T w;};
pair<T, vector<int>> dmst(int n, int r, vector<Edge> E) {
	int m = E.size();
	vector<priority_queue<ptt, vector<ptt>, greater<ptt>>> Q(n);
	vector<T> L(n);
	for(int i = 0; i < (int)E.size(); ++i) Q[E[i].v].push({ E[i].w, i });
	T wgh = 0;
	UF1 uf1(n + m); UF2 uf2(n + m);
	vector<ptt> P(n, {-1, -1});
	vector<int> R;
	queue<int> V;
	for(int i = 0; i < n; ++i) if(i != r) V.push(i);
	while(V.size()) {
		int q = V.front(); V.pop();
		while(q == uf2.fnd(E[Q[q].top().ss].u)) Q[q].pop();
		auto [w, e] = Q[q].top(); Q[q].pop();
		w += L[q];
		wgh += w;
		P[q] = {e, w};
		if(!uf1.uni(E[e].u, E[e].v)) {
			int x = q, y = x;
			vector<int> cyc;
			do { cyc.push_back(y); y = uf2.fnd(E[P[y].ff].u); } while(x != y);
			int N = P.size();
			Q.emplace_back();
			L.push_back(0);
			P.push_back({-1, -1});
			R.push_back(uf2.his.size());
			V.push(N);
			uf1.uni(cyc[0], N);
			for(auto i : cyc) uf2.uni(i, N);
			for(int i = 1; i < (int)cyc.size(); ++i) if(Q[cyc[0]].size() < Q[cyc[i]].size()) swap(cyc[0], cyc[i]);
			swap(Q[cyc[0]], Q[N]);
			swap(L[cyc[0]], L[N]);
			L[N] -= P[cyc[0]].ss;
			for(int i = 1; i < (int)cyc.size(); ++i) {
				while(Q[cyc[i]].size()) {
					auto [_w, _e] = Q[cyc[i]].top(); Q[cyc[i]].pop();
					Q[N].push({ _w + L[cyc[i]] - L[N] - P[cyc[i]].ss, _e });
				}
			}
		}
	}
	for(int i = (int)P.size() - 1; i >= n; --i) {
		while(uf2.his.size() > R[i - n]) uf2.par[uf2.his.back().ff] = uf2.his.back().ss, uf2.his.pop_back();
		P[uf2.fnd(E[P[i].ff].v)].ff = P[i].ff;
	}
	vector<int> ret(n);
	for(int i = 0; i < n; ++i) ret[i] = (P[i].ff == -1 ? i : E[P[i].ff].u);
	return {wgh, ret};
}


// https://judge.yosupo.jp/problem/directedmst
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m, r; cin >> n >> m >> r;
	vector<Edge> E;
	for(int i = 0; i < m; ++i)
	{
		int x, y, v; cin >> x >> y >> v;
		E.push_back({x, y, v});
	}
	auto [wgh, ret] = dmst(n, r, E);
	cout << wgh << '\n';
	for(int i = 0; i < n; ++i) cout << ret[i] << ' ';
}