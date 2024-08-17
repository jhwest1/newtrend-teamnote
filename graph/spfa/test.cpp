#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

const int SV = 101010;
typedef long long T;
namespace spfa
{
	// INF should be larger than E|X|
	const T INF = (T)1e18 + 100;
	int n;
	vector<pair<int, T>> gph[SV];
	T dist[SV];
	int par[SV], in[SV];
	bool inq[SV];
	void init(int _n) {
		n = _n;
		for(int i = 1; i <= n; ++i) gph[i].clear();
	}
	void add_edge(int u, int v, T w) { gph[u].push_back({v, w}); }
	bool is_cycle() {
		fill(in + 1, in + n + 1, 0);
		for(int i = 1; i <= n; ++i) if(par[i] != 0) ++in[par[i]];
		queue<int> Q;
		for(int i = 1; i <= n; ++i) if(!in[i]) Q.push(i);
		while(Q.size()) {
			int x = Q.front(); Q.pop();
			if(par[x] != 0 && !--in[par[x]]) Q.push(par[x]);
		}
		// in order to restore cycle, choose any node s.t. in[i] != 0, then i belongs to cycle
		// then, follow par[i] until it returns to i
		for(int i = 1; i <= n; ++i) if(in[i]) return true;
		return false;
	}
	bool shortest_path(int s) {
		fill(dist + 1, dist + n + 1, INF);
		fill(par + 1, par + n + 1, 0);
		fill(inq + 1, inq + n + 1, false);
		deque<int> Q;
        if(s==-1) {
            for(int i=1; i<=n; i++) {
                dist[i]=0;
                inq[i]=true;
                Q.push_back(i);
            }
        }
        else {
            dist[s] = 0;
            inq[s] = true;
            Q.push_back(s);
        }
		int cnt = 0;
		while(Q.size()) {
			int x = Q.front(); Q.pop_front();
			inq[x] = false;
			for(auto [y, w] : gph[x]) {
				if(dist[y] > dist[x] + w) {
					dist[y] = max(dist[x] + w, -INF);
					par[y] = x;
					if(!inq[y]) {
						inq[y] = true;
						if(Q.size() && dist[Q[0]] > dist[y]) Q.push_front(y);
						else Q.push_back(y);
					}
				}
			}
			// comment this line if negative cycle detection is not required 
			if(++cnt % n == 0 && is_cycle()) return false;
		}
		return true;
	}
}

int main()
{
    int TC;
    scanf("%d", &TC);
    while(TC--)
    {
        int N, M, K;
        scanf("%d%d%d", &N, &M, &K);
        spfa::init(N);
        ll x=1e9;
        for(int i=1; i<=M; i++)
        {
            int u, v, w;
            scanf("%d%d%d", &u, &v, &w);
            spfa::add_edge(u, v, w*x);
            spfa::add_edge(v, u, w*x);
        }
        for(int i=1; i<=K; i++)
        {
            int u, v, w;
            scanf("%d%d%d", &u, &v, &w);
            spfa::add_edge(u, v, -w*x);
        }
        if(spfa::shortest_path(-1)) printf("NO\n");
        else printf("YES\n");
    }
}