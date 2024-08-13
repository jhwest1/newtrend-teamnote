const int SV = 505050;
const int SE = 505050;
namespace bcc
{
	vector<pii> gph[SV];
	pii E[SE];
	vector<int> color[SV], bcc[SV];
	int ecol[SE];
	bool chc[SV];
	int in[SV];
	int cnt, bcnt;
	vector<int> S;

	int n, m;
	
	void init(int _n, int _m)
	{
		n = _n; m = _m;
		for(int i = 1; i <= n; ++i) gph[i].clear(), color[i].clear(), bcc[i].clear();
		for(int i = 1; i <= m; ++i) ecol[i] = 0, E[i] = {0, 0}; 
		fill(chc + 1, chc + n + 1, false);
		fill(in + 1, in + n + 1, 0);
		cnt = 0; bcnt = 0;
		S.clear();
	}
	
	void add_edge(int u, int v, int i)
	{
		gph[u].push_back({v, i});
		gph[v].push_back({u, i});
		E[i] = {u, v};
	}
	
	int dfs(int x, int p)
	{
		int ret = in[x] = ++cnt;

		for(auto [y, e] : gph[x]) if(p != e)
		{
			if(!in[y])
			{
				S.push_back(e);
				int t = dfs(y, e);
				ret = min(ret, t);
				if(t >= in[x])
				{
					++bcnt;
					while(1)
					{
						int f = S.back(); S.pop_back();
						ecol[f] = bcnt;
						if(e == f) break;
					}
				}
			}
			else if(in[x] > in[y])
			{
				S.push_back(e);
				ret = min(ret, in[y]);
			}
		}

		return ret;
	}

    // bcnt : number of bcc
    // bcc[c] : vertices of color c
    // color[v] : colors of vertex v
    // ecol[e] : color of edge e
	void get_bcc()
	{
		for(int i = 1; i <= n; ++i) if(!in[i]) dfs(i, -1);
		for(int i = 1; i <= m; ++i)
		{
			auto [x, y] = E[i];
			bcc[ecol[i]].push_back(x);
			bcc[ecol[i]].push_back(y);
		}
		// uncomment this if isolated vertex is also a bcc.
		// for(int i = 1; i <= n; ++i) if(gph[i].size() == 0) bcc[++bcnt].push_back(i);
		for(int i = 1; i <= bcnt; ++i)
		{
			sort(bcc[i].begin(), bcc[i].end());
			bcc[i].resize(unique(bcc[i].begin(), bcc[i].end()) - bcc[i].begin());
			for(auto j : bcc[i]) color[j].push_back(i);
		}
	}
}