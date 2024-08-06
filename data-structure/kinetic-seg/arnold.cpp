namespace KST
{
    const ll INF = 7e18;
    struct Line { ll a, b; };
    ll divfloor(ll u, ll d) { return u/d - ((u^d)<0 && u%d); }

    // p(0) <= q(0) must hold ( min : <= , max : >= )
    // minimum t>0 s.t. p(t) > q(t) ( min : > , max : < )
    ll cross(Line p, Line q)
    {
        assert(p.b<=q.b); // min : <= , max : >=
        if(p.a<=q.a) return INF; // min : <= , max : >=
        return divfloor(p.b-q.b, q.a-p.a)+1;
    }

    struct SEG
    {
        vector<Line> tree;
        vector<ll> melt; // minimum time left for next cross in subtree
        vector<ll> lazy;

        SEG(int N)
        {
            tree = vector<Line>(N*4+10, {0, INF}); // min : INF , max : -INF
            melt = vector<ll>(N*4+10, INF);
            lazy = vector<ll>(N*4+10, 0);
        }

        // Apply update value k to subtree of node
        // update to node itself, and to lazy value
        void apply(int node, ll k)
        {
            assert(k<melt[node]);
            tree[node].b+=tree[node].a*k;
            melt[node]-=k;
            lazy[node]+=k;
        }

        // Propagate lazy value to children, and initialize lazy
        // should be called before going down to children
        void prop(int node, int tl, int tr)
        {
            if(tl!=tr)
            {
                apply(node*2, lazy[node]);
                apply(node*2+1, lazy[node]);
            }
            lazy[node]=0;
        }

        // Recalculate value of node from children
        // lazy must be empty
        void recalc(int node)
        {
            assert(lazy[node]==0);
            melt[node]=min(melt[node*2], melt[node*2+1]);
            if(tree[node*2].b<=tree[node*2+1].b) // min : <= , max : >=
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

        // B[i]+=A[i]*t for all i in [l, r]
        // Ammortized O(Qlog^3N) if range heaten
        // Ammortized O(Qlog^2N) if entire heaten
        void heaten(int node, int tl, int tr, int l, int r, ll t)
        {
            if(r<tl || tr<l) return;
            if(l<=tl && tr<=r && melt[node]>t)
            {
                apply(node, t);
                return;
            }
            prop(node, tl, tr);
            int mid=tl+tr>>1;
            heaten(node*2, tl, mid, l, r, t);
            heaten(node*2+1, mid+1, tr, l, r, t);
            recalc(node);
        }

        // get minimum/maximum B[i] for i in [l, r]
        // O(QlogN)
        ll query(int node, int tl, int tr, int l, int r)
        {
            if(r<tl || tr<l) return INF; // min : INF , max : -INF
            if(l<=tl && tr<=r) return tree[node].b;
            prop(node, tl, tr);
            int mid=tl+tr>>1;
            return min(query(node*2, tl, mid, l, r), query(node*2+1, mid+1, tr, l, r)); // min : min() , max : max()
        }

        // update A[pos], B[pos]
        // O(QlogN)
        void update(int node, int tl, int tr, int pos, Line p)
        {
            if(tl==tr)
            {
                tree[node]=p;
                return;
            }
            prop(node, tl, tr);
            int mid=tl+tr>>1;
            if(pos<=mid) update(node*2, tl, mid, pos, p);
            else update(node*2+1, mid+1, tr, pos, p);
            recalc(node);
        }
    };
}