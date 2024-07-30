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

    void add_edge(int x, int y, T c, bool dir) { // directed edge : dir = true, undirected edge : dir = false
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