struct Line { ll a, b; };
ll divfloor(ll u, ll d) { return u/d - ((u^d)<0 && u%d); }

// change if maximum
// p(0) <= q(0)
// minimum t>0 s.t. p(t) > q(t)
ll cross(Line p, Line q)
{
    assert(p.b<=q.b);
    if(p.a<=q.a) return INF;
    return divfloor(p.b-q.b, q.a-p.a)+1;
}

struct SEG
{
    Line tree[MAXN*4+10];
    ll melt[MAXN*4+10]; // minimum time left for next cross in subtree
    ll lazy[MAXN*4+10];

    void busy(int node, int tl, int tr)
    {
        assert(lazy[node]<melt[node]);
        tree[node].b+=tree[node].a*lazy[node];
        melt[node]-=lazy[node];
        if(tl!=tr) lazy[node*2]+=lazy[node], lazy[node*2+1]+=lazy[node];
        lazy[node]=0;
    }

    void init(int node, int tl, int tr)
    {
        tree[node]={0, INF};
        melt[node]=INF;
        lazy[node]=0;
        if(tl==tr) return;
        int mid=tl+tr>>1;
        init(node*2, tl, mid);
        init(node*2+1, mid+1, tr);
    }

    void recalc(int node)
    {
        melt[node]=min(melt[node*2], melt[node*2+1]);
        if(tree[node*2].b<=tree[node*2+1].b) // change if maximum
        {
            tree[node]=tree[node*2];
            melt[node]=min(melt[node], cross(tree[node*2], tree[node*2+1]));
        }
        else
        {
            tree[node]=tree[node*2+1];
            melt[node]=min(melt[node], cross(tree[node*2+1], tree[node*2]));
        }
    }

    // Ammortized O(Qlog^3N) if range heaten
    // Ammortized O(Qlog^2N) if entire heaten
    void heaten(int node, int tl, int tr, int l, int r, ll t) // B[i]+=A[i]*t for all i in [l, r]
    {
        busy(node, tl, tr);
        if(r<tl || tr<l) return;
        if(l<=tl && tr<=r && melt[node]>t)
        {
            lazy[node]=t;
            busy(node, tl, tr);
            return;
        }
        int mid=tl+tr>>1;
        heaten(node*2, tl, mid, l, r, t);
        heaten(node*2+1, mid+1, tr, l, r, t);
        recalc(node);
    }

    // O(QlogN)
    ll query(int node, int tl, int tr, int l, int r) // get minimum B[i] for i in [l, r]
    {
        busy(node, tl, tr);
        if(r<tl || tr<l) return INF;
        if(l<=tl && tr<=r) return tree[node].b;
        int mid=tl+tr>>1;
        return min(query(node*2, tl, mid, l, r), query(node*2+1, mid+1, tr, l, r));
    }

    // O(QlogN)
    void update(int node, int tl, int tr, int pos, Line p)
    {
        busy(node, tl, tr);
        if(tl==tr)
        {
            tree[node]=p;
            return;
        }
        int mid=tl+tr>>1;
        if(pos<=mid) update(node*2, tl, mid, pos, p);
        else update(node*2+1, mid+1, tr, pos, p);
        recalc(node);
    }
}seg;