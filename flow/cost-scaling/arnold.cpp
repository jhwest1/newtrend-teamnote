// 0-based
struct HLPP {
    // O(V^2 sqrt(E))
    // add_edge()를 호출해서 간선 추가
    // hlpp.flow(s, t, n)을 호출해서 |V|=n 일 때 s에서 t로의 유량 구하기

	typedef long long T;

    const static int MAXV = 1200;
    struct edge { int y; T c; int r; };

    int n;
    vector<edge> gph[MAXV];
    T ex[MAXV];
    vector<int> act[MAXV * 2];
    int h[MAXV];
    vector<int> st[MAXV];
    int mxh;

    HLPP(int _n) {
        n = _n;
		for (int i = 0; i < n; ++i)
		{
            ex[i] = h[i] = 0;
            st[i].clear();
            gph[i].clear();
		}
        for(int i=0; i < n * 2; i++) act[i].clear();
		mxh = 0;
    }

    void add_edge(int x, int y, T c, bool dir = true) { // directed edge : dir = true, undirected edge : dir = false
        gph[x].push_back({y, c, (int)gph[y].size()});
        gph[y].push_back({x, dir ? 0 : c, (int)gph[x].size() - 1});
    }

    void push(int v, int e) {
        auto &w = gph[v][e];
        T t = min(ex[v], w.c);
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

    T flow(int s, int t) {
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

struct Circulation {
    typedef long long T;

    const T INF = numeric_limits<T>::max() / 2;

    int n;
    T lbsum = 0;
    HLPP mf; // any max flow algorithm
    
    // dummy src = n, dummy snk = n+1
    Circulation(int k) : mf(k+2) {
        n = k;
        lbsum = 0;
    }

    void add_edge(int s, int e, T l, T r){
        mf.add_edge(s, e, r - l);
        if(l > 0){
            mf.add_edge(n, e, l);
            mf.add_edge(s, n + 1, l);
            lbsum += l;
        }
        else{
            mf.add_edge(n, s, -l);
            mf.add_edge(e, n + 1, -l);
            lbsum += -l;
        }
    }
    bool solve(int s, int e){
        // mf.add_edge(e, s, INF); // to reduce as maxflow with lower bounds, in circulation problem skip this line
        return lbsum == mf.flow(n, n + 1);
        // to get maximum LR flow, run maxflow from s to e again
    }
};

struct MinCostCirculation {
    // O(V^3 log(VC))

    typedef long long T;

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
    Circulation circ;
    vector<vector<Edge>> gph;
    vector<T> p;

    MinCostCirculation(int k) : circ(k), gph(k), p(k) { n = k; }
    void add_edge(int s, int e, T l, T r, T cost){
        estk.push_back({s, e, l, r, cost});
    }
    pair<bool, T> solve(){
        for(auto &i : estk){
            if(i.s != i.e) circ.add_edge(i.s, i.e, i.l, i.r);
        }
        if(!circ.solve(-1, -1)){
            return make_pair(false, T(0));
        }

        vector<int> ptr(n);
        T eps = 0;
        for(auto &i : estk){
            T curFlow;
            if(i.s != i.e) curFlow = i.r - circ.mf.gph[i.s][ptr[i.s]].c;
            else curFlow = i.r;
            int srev = gph[i.e].size();
            int erev = gph[i.s].size();
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
            queue<int> Q;
            auto push = [&](Edge &e, int src, T flow){
                e.rem -= flow;
                gph[e.pos][e.rev].rem += flow;
                excess[src] -= flow;
                excess[e.pos] += flow;
                if(excess[e.pos] <= flow && excess[e.pos] > 0){
                    Q.push(e.pos);
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
            while(!Q.empty()){
                int x = Q.front();
                Q.pop();
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
};