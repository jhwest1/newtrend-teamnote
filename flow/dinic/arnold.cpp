namespace Dinic
{
    const int MAXV = 1200;
    const int INF = 2147483647;

    struct Edge { int v, c, r; };

    int N, src, snk;
    vector<Edge> adj[MAXV+10];
    int lvl[MAXV+10], pos[MAXV+10];
    int lim;

    void init(int _N)
    {
        N=_N;
        for(int i=1; i<=N; i++) adj[i].clear();
    }

    void add_edge(int u, int v, int c)
    {
        adj[u].push_back({v, c, adj[v].size()});
        adj[v].push_back({u, 0, adj[u].size()-1});
    }

    bool bfs()
    {
        for(int i=1; i<=N; i++) lvl[i]=0;
        queue<int> Q;
        Q.push(src);
        lvl[src]=1;
        while(!Q.empty())
        {
            int now=Q.front(); Q.pop();
            for(auto [nxt, c, r] : adj[now])
            {
                if(lvl[nxt] || c<lim) continue;
                Q.push(nxt);
                lvl[nxt]=lvl[now]+1;
            }
        }
        return lvl[snk];
    }

    int dfs(int now, int flow)
    {
        if(now==snk) return flow;
        for(; pos[now]<adj[now].size(); pos[now]++)
        {
            auto &[nxt, c, r] = adj[now][pos[now]];
            if(lvl[nxt]!=lvl[now]+1 || !c) continue;
            int f=dfs(nxt, min(flow, c));
            if(f)
            {
                c-=f;
                adj[nxt][r].c+=f;
                return f;
            }
        }
        return 0;
    }

    ll flow(int _src, int _snk)
    {
        src=_src; snk=_snk;

        ll ans=0;
        for(lim=(1<<30); lim>0; lim>>=1)
        {
            while(bfs())
            {
                for(int i=1; i<=N; i++) pos[i]=0;
                while(1)
                {
                    int t=dfs(src, INF);
                    if(!t) break;
                    ans+=t;
                }
            }
        }
        return ans;
    }
}