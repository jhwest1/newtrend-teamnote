#include <bits/stdc++.h>
#define sz(v) ((int)(v).size())
#define all(v) (v).begin(), (v).end()
using namespace std;
typedef long long lint;
typedef pair<lint, lint> pi;
typedef long long ll;

const int mod = 1e9 + 7;
// 0-based
struct HLPP {
    // O(V^2 sqrt(E))
    // add_edge()를 호출해서 간선 추가
    // hlpp.flow(s, t, n)을 호출해서 |V|=n 일 때 s에서 t로의 유량 구하기

    const static int MAXV = 1010;
    struct edge { int y, c, r; };

    int n;
    vector<edge> gph[MAXV];
    ll ex[MAXV];
    vector<int> act[MAXV * 2];
    int h[MAXV];
    vector<int> st[MAXV];
    int mxh;

    HLPP(int _n) {
        n=_n;
		for(int i=0; i<n; i++)
		{
			ex[i]=0;
			h[i]=0;
		}
		mxh=0;
        // // TODO
    }

    void add_edge(int x, int y, int c) {
		// cout << x << " " << y << " " << c << "\n";
        gph[x].push_back({y, c, (int)gph[y].size()});
        gph[y].push_back({x, 0, (int)gph[x].size() - 1});
    }

    void push(int v, int e) {
        auto &w = gph[v][e];
        int t = min(ex[v], 1ll * w.c);
        if (!t || h[w.y] >= h[v]) return;
        if (!ex[w.y]) act[h[w.y]].push_back(w.y);
        w.c -= t;
        gph[w.y][w.r].c += t;
        ex[v] -= t;
        ex[w.y] += t;
        if (w.c) st[v].push_back(e);
    }
    
    void relabel(int v) {
        ++h[v];
        act[h[v]].push_back(v);
        mxh = max(mxh, h[v]);
        st[v].clear();
        for (int i = 0; i < (int)gph[v].size(); ++i) {
            if (gph[v][i].c && h[gph[v][i].y] < h[v]) {
                st[v].push_back(i);
            }
        }
    }

    ll flow(int s, int t) {
        h[s] = n;
        for (auto &v : gph[s]) {
            if (v.c && !ex[v.y]) act[0].push_back(v.y);
            ex[s] -= v.c;
            ex[v.y] += v.c;
            gph[v.y][v.r].c += v.c;
            v.c = 0;
        }
        while (mxh >= 0) {
            if (act[mxh].empty()) { --mxh; continue; }
            int x = act[mxh].back();
            act[mxh].pop_back();
            if (x == t) continue;
            while (ex[x] > 0 && st[x].size()) {
                int y = st[x].back();
                st[x].pop_back();
                push(x, y);
            }
            if (ex[x] > 0) relabel(x);
        }
        return ex[t];
    }
};


template <class T> struct Circulation {
    const T INF = numeric_limits<T>::max() / 2;
    T lowerBoundSum = 0;
    HLPP mf;
 
    // Initialize for n vertices
    Circulation(int k) : mf(k+2) {  }
    void add_edge(int s, int e, T l, T r){
		// cout << "WTF " << s << " " << e << " " << l << " " << r << "\n";
        mf.add_edge(s + 2, e + 2, r - l);
        if(l > 0){
            mf.add_edge(0, e + 2, l);
            mf.add_edge(s + 2, 1, l);
        	lowerBoundSum += l;
        }
        else{
        	mf.add_edge(0, s + 2, -l);
        	mf.add_edge(e + 2, 1, -l);
        	lowerBoundSum += -l;
		}
    }
    bool solve(int s, int e){
 //       mf.add_edge(e+2, s+2, INF); // to reduce as maxflow with lower bounds, in circulation problem skip this line
        return lowerBoundSum == mf.flow(0, 1);
        // to get maximum LR flow, run maxflow from s+2 to e+2 again
    }
};

template <class T> struct MinCostCirculation {
	const int SCALE = 3; // scale by 1/(1 << SCALE)
    const T INF = numeric_limits<T>::max() / 2;
	struct EdgeStack {
		int s, e;
		T l, r, cost;
	};
	struct Edge {
		int pos, rev;
		T rem, cap, cost;
	};
	int n;
	vector<EdgeStack> estk;
	Circulation<T> circ;
	vector<vector<Edge>> gph;
	vector<T> p;
    MinCostCirculation(int k) : circ(k), gph(k), p(k) { n = k; }
	void add_edge(int s, int e, T l, T r, T cost){
		// cout << "?\n";
		estk.push_back({s, e, l, r, cost});
	}
	pair<bool, T> solve(){
		for(auto &i : estk){
			if(i.s != i.e) circ.add_edge(i.s, i.e, i.l, i.r);
		}
		if(!circ.solve(-1, -1)){
			return make_pair(false, T(0));
		}
		// cout << "HIHI\n"
		vector<int> ptr(n);
		T eps = 0;
		for(auto &i : estk){
			T curFlow;
			if(i.s != i.e) curFlow = i.r - circ.mf.gph[i.s + 2][ptr[i.s]].c;
			else curFlow = i.r;
			int srev = sz(gph[i.e]);
			int erev = sz(gph[i.s]);
			if(i.s == i.e) srev++;
			gph[i.s].push_back({i.e, srev, i.r - curFlow, i.r, i.cost * (n + 1)});
			gph[i.e].push_back({i.s, erev, -i.l + curFlow, -i.l, -i.cost * (n + 1)});
			eps = max(eps, abs(i.cost) * (n + 1));
			if(i.s != i.e){
				ptr[i.s] += 2;
				ptr[i.e] += 2;
			}
		}
		while(eps > 1){
			eps = max(T(1), eps >> SCALE);
			auto cost = [&](Edge &e, int s, int t){
				return e.cost + p[s] - p[t];
			};
			vector<T> excess(n);
			queue<int> que;
			auto push = [&](Edge &e, int src, T flow){
				e.rem -= flow;
				gph[e.pos][e.rev].rem += flow;
				excess[src] -= flow;
				excess[e.pos] += flow;
				if(excess[e.pos] <= flow && excess[e.pos] > 0){
					que.push(e.pos);
				}
			};
			auto relabel = [&](int v){
				p[v] = -INF;
				for(auto &e : gph[v]){
					if(e.rem > 0){
						p[v] = max(p[v], p[e.pos] - e.cost - eps);
					}
				}
			};
			for(int i = 0; i < n; i++){
				for(auto &j : gph[i]){
					if(j.rem > 0 && cost(j, i, j.pos) < 0){
						push(j, i, j.rem);
					}
				}
			}
			while(sz(que)){
				int x = que.front();
				que.pop();
				while(excess[x] > 0){
					for(auto &e : gph[x]){
						if(e.rem > 0 && cost(e, x, e.pos) < 0){
							push(e, x, min(e.rem, excess[x]));
							if(excess[x] == 0) break;
						}
					}
					if(excess[x] == 0) break;
					relabel(x);
				}
			}
		}
		T ans = 0;
		for(int i = 0; i < n; i++){
			for(auto &j : gph[i]){
				j.cost /= (n + 1);
				ans += j.cost * (j.cap - j.rem);
			}
		}
		return make_pair(true, ans / 2);
	}
	void bellmanFord(){
		fill(all(p), T(0));
		bool upd = 1;
		while(upd){
			upd = 0;
			for(int i = 0; i < n; i++){
				for(auto &j : gph[i]){
					if(j.rem > 0 && p[j.pos] > p[i] + j.cost){
						p[j.pos] = p[i] + j.cost;
						upd = 1;
					}
				}
			}
		}
	}
};

ostream& operator<<(ostream&os,__int128 i){
	if(i==0)
		return os<<0;
	static char buf[100];
	if(i<0){
		os<<"-";
		i=-i;
	}
	int p=0;
	while(i){
		buf[p++]='0'+i%10;
		i/=10;
	}
	reverse(buf,buf+p);
	buf[p]=0;
	return os<<buf;
}

int main(){
	// ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	int n, m;
	cin >> n >> m;
	MinCostCirculation<lint> mcc(n + 1);
	vector<int> ptr(n + 1);
	for(int i = 0; i < n; i++){
		int x; cin >> x;
		mcc.add_edge(n, i, x, x, 0);
		ptr[n]++; ptr[i]++;
	}
	vector<pi> track;
	for(int i = 0; i < m; i++){
		int s, t, l, u, c;
		cin >> s >> t >> l >> u >> c;
		mcc.add_edge(s, t, l, u, c);
		track.emplace_back(s, ptr[s]);
		ptr[s]++;
		ptr[t]++;
	}
	auto sln = mcc.solve();
	if(!sln.first){
		cout << "infeasible\n";
		return 0;
	}
	mcc.bellmanFord();
	vector<lint> flows;
	__int128 ret = 0;
	for(auto &[v, p] : track){
		flows.push_back(mcc.gph[v][p].cap - mcc.gph[v][p].rem);
		ret += mcc.gph[v][p].cost * flows.back();
	}
	cout << ret << "\n";
	for(int i = 0; i < n; i++) cout << mcc.p[i] << "\n";
	for(auto &i : flows) cout << i << "\n";
}
