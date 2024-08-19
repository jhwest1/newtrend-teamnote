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

// call init(|V|) first, add_edge(u, v, x), then kth_walk(s, e, K)
// O(|E|log|E|+klogk) in total
// if there is negative edge, O((SPFA time) + |E|log|E| + klogk)
// multi-edges, loops are allowed.
// ALERT: NODES ARE 0-BASED!!!!!!!!
// ALERT: max answer = K|E||X| could be too large.
typedef long long T;
typedef pair<T, T> ptt;
const int SV = 505050, HSIZE = 20202020; // HSIZE >= 2|E|log|E|
const T INF = (T)1e18; // INF >= |E||X| (not need to be larger than K|E||X|)
int n;
vector<pair<int, T>> gph[SV], rgph[SV];
struct Lheap {
	struct Node {
		ptt x;
		int l, r, s;
		Node(void) : x({0, 0}), l(0), r(0), s(0) {}
		Node(ptt x) : x(x), l(0), r(0), s(1) {}
	}h[HSIZE];
	int cnt = 1;
	int mk(ptt x) { h[cnt] = Node(x); return cnt++; }
	void norm(int x) {
		if(h[h[x].l].s < h[h[x].r].s) swap(h[x].l, h[x].r); 
		h[x].s = h[h[x].r].s + 1;
	}
	int mrge(int x, int y) {
		if(!x || !y) return x ^ y;
		if(h[x].x > h[y].x) swap(x, y);
		int ret = mk(h[x].x);
		h[ret].l = h[x].l;
		h[ret].r = mrge(h[x].r, y);
		norm(ret);
		return ret;
	}
}hp;
void init(int _n) {
	n = _n;
	for(int i = 0; i < n; ++i) gph[i].clear();
	for(int i = 0; i < n; ++i) rgph[i].clear();
	hp.cnt = 1;
}
void add_edge(int u, int v, T x) {
	gph[u].push_back({v, x});
	rgph[v].push_back({u, x});
}
// return less than K elements if there is no such walk
vector<T> kth_walk(int s, int e, int K) {
	int nxt[n]; vector<int> top; bool vst[n]{};
	T dst[n]; fill(dst, dst + n, INF); dst[e] = 0;
	typedef tuple<T, int, int> tlii;
	// if there is no negative edge
	priority_queue<tlii, vector<tlii>, greater<tlii>> Q;
	Q.push({0, e, -1});
	while(Q.size()) {
		auto [d, x, p] = Q.top(); Q.pop();
		if(vst[x]) continue;
		vst[x] = true; nxt[x] = p;
		top.push_back(x);
		for(auto [y, c] : rgph[x]) if(!vst[y] && dst[y] > d + c) dst[y] = d + c, Q.push({d + c, y, x});
	}
	if(dst[s] >= INF) return vector<T>();
	bool chc[n]{};
	int rt[n]{};
	for(auto x : top) if(dst[x] < INF) {
		if(nxt[x] != -1) rt[x] = rt[nxt[x]];
		for(auto [y, c] : gph[x]) if(dst[y] < INF) {
			if(!chc[x] && y == nxt[x] && dst[x] == c + dst[y]) { chc[x] = true; continue; }
			rt[x] = hp.mrge(rt[x], hp.mk({c + dst[y] - dst[x], y}));
		}
	}
	vector<T> ret({dst[s]});
	priority_queue<ptt, vector<ptt>, greater<ptt>> PQ;
	if(rt[s]) PQ.push({hp.h[rt[s]].x.ff, rt[s]});
	while((int)ret.size() < K && PQ.size()) {
		auto [d, x] = PQ.top(); PQ.pop();
		ret.push_back(dst[s] + d);
		if(rt[hp.h[x].x.ss]) PQ.push({d + hp.h[rt[hp.h[x].x.ss]].x.ff, rt[hp.h[x].x.ss]});
		if(hp.h[x].l) PQ.push({d - hp.h[x].x.ff + hp.h[hp.h[x].l].x.ff, hp.h[x].l});
		if(hp.h[x].r) PQ.push({d - hp.h[x].x.ff + hp.h[hp.h[x].r].x.ff, hp.h[x].r});
	}
	return ret;
}


//https://judge.yosupo.jp/problem/k_shortest_walk
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m, s, e, K; cin >> n >> m >> s >> e >> K;
	init(n);
	for(int i = 0; i < m; ++i)
	{
		int x, y, v; cin >> x >> y >> v;
		add_edge(x, y, v);
	}
	auto ret = kth_walk(s, e, K);
	for(auto i : ret) cout << i << '\n';
}