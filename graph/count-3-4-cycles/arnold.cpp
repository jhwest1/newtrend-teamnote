namespace count_3_4_cyc
{
    const int MAXV = 1e5;

    int N, M;
    vector<int> adj[MAXV+10];
    int cnt[MAXV+10];

    void init(int _N, int _M)
    {
        N=_N; M=_M;
        for(int i=1; i<=N; i++)
        {
            adj[i]=vector<int>();
            cnt[i]=0;
        }
    }

    void add_edge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<array<int, 3>> find_3_cycle()
    {
        vector<array<int, 3>> ans;
        for(int u=1; u<=N; u++)
        {
            for(auto v : adj[u]) cnt[v]=1;
            for(auto v : adj[u])
            {
                if(pii(adj[v].size(), v)>pii(adj[u].size(), u)) continue; // v < u
                for(auto w : adj[v])
                {
                    if(pii(adj[w].size(), w)>pii(adj[v].size(), v)) continue; // w < v
                    if(cnt[w]) ans.push_back({u, v, w});
                }
            }
            for(auto v : adj[u]) cnt[v]=0;
        }
        return ans;
    }

    ll count_4_cycle()
    {
        ll ans=0;
        for(int u=1; u<=N; u++)
        {
            vector<int> V;
            for(auto v : adj[u])
            {
                if(pii(adj[v].size(), v)>pii(adj[u].size(), u)) continue; // v < u
                for(auto w : adj[v])
                {
                    if(pii(adj[w].size(), w)>=pii(adj[u].size(), u)) continue; // w < u
                    // w - v - u
                    if(!cnt[w]) V.push_back(w);
                    cnt[w]++;
                }
            }
            for(auto it : V)
            {
                ans+=1ll*cnt[it]*(cnt[it]-1)/2;
                cnt[it]=0;
            }
        }
        return ans;
    }
}