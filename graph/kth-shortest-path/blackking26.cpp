namespace kth_shortest_path
{
	const int MAXV = 505050;
	const int MAXE = 505050;
	const int HSIZE = 20202020;
	int n;
	vector<pii> gph[MAXV];
	vector<pii> rgph[MAXV];

	struct Lheap
	{
		struct Node
		{
			pll x;
			int l, r, s;
			Node(void) : x({0, 0}), l(0), r(0), s(0) {}
			Node(pll x) : x(x), l(0), r(0), s(1) {}
		}h[HSIZE];
		int cnt = 1;

		int mk(pll x) { h[cnt] = Node(x); return cnt++; }
		void norm(int x)
		{
			if(h[h[x].l].s < h[h[x].r].s) swap(h[x].l, h[x].r); 
			h[x].s = h[h[x].r].s + 1;
		}
		int mrge(int x, int y)
		{
			if(!x || !y) return x ^ y;
			if(h[x].x > h[y].x) swap(x, y);
			int ret = mk(h[x].x);
			h[ret].l = h[x].l;
			h[ret].r = mrge(h[x].r, y);
			norm(ret);
			return ret;
		}
	}hp;

	void init(int _n)
	{
		n = _n;
		for(int i = 0; i < n; ++i) gph[i].clear();
		for(int i = 0; i < n; ++i) rgph[i].clear();
		hp.cnt = 1;
	}
	void add_edge(int u, int v, int x)
	{
		gph[u].push_back({v, x});
		rgph[v].push_back({u, x});
	}
	// return -1 if there is no such walk
	// every weight must be positive
	vector<ll> kth_path(int s, int e, int K)
	{
		int nxt[n];
		typedef tuple<ll, int, int> tlii;
		priority_queue<tlii, vector<tlii>, greater<tlii>> Q;
		ll dst[n]; fill(dst, dst + n, INF); dst[e] = 0;
		bool vst[n]{};
		Q.push({0, e, -1});
		vector<int> top;

		while(Q.size())
		{
			auto [d, x, p] = Q.top(); Q.pop();
			if(vst[x]) continue;
			vst[x] = true;
			nxt[x] = p;
			top.push_back(x);
			for(auto [y, c] : rgph[x]) if(!vst[y] && dst[y] > d + c) dst[y] = d + c, Q.push({d + c, y, x});
		}

		if(dst[s] >= INF) return vector<ll>(K, -1);

		bool chc[n]{};
		int rt[n]{};

		for(auto x : top) if(dst[x] < INF)
		{
			if(nxt[x] != -1) rt[x] = rt[nxt[x]];
			for(auto [y, c] : gph[x]) if(dst[y] < INF)
			{
				if(!chc[x] && y == nxt[x] && dst[x] == c + dst[y])
				{
					chc[x] = true;
					continue;
				}
				rt[x] = hp.mrge(rt[x], hp.mk({c + dst[y] - dst[x], y}));
			}
		}

		vector<ll> ret({dst[s]});
		priority_queue<pll, vector<pll>, greater<pll>> PQ;
		if(rt[s]) PQ.push({hp.h[rt[s]].x.ff, rt[s]});
		while((int)ret.size() < K && PQ.size())
		{
			auto [d, x] = PQ.top(); PQ.pop();
			ret.push_back(dst[s] + d);
			if(rt[hp.h[x].x.ss]) PQ.push({d + hp.h[rt[hp.h[x].x.ss]].x.ff, rt[hp.h[x].x.ss]});
			if(hp.h[x].l) PQ.push({d - hp.h[x].x.ff + hp.h[hp.h[x].l].x.ff, hp.h[x].l});
			if(hp.h[x].r) PQ.push({d - hp.h[x].x.ff + hp.h[hp.h[x].r].x.ff, hp.h[x].r});
		}
		while((int)ret.size() < K) ret.push_back(-1);

		return ret;
	}
}
