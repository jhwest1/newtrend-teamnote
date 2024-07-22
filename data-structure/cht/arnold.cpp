namespace CHT
{
    typedef __int128 dll;
    struct Line { ll a, b; };
    struct Frac
    {
        ll u, d;
        Frac(ll _u, ll _d) : u(_u), d(_d) { if(d<0) u=-u, d=-d; }
        bool operator < (const Frac &ot) const { return (dll)u*ot.d < (dll)ot.u*d; }
        bool operator > (const Frac &ot) const { return (dll)u*ot.d > (dll)ot.u*d; }
    };
    ll divfloor(ll u, ll d) { return u/d - ((u^d)<0 && u%d); }
    ll divceil(ll u, ll d) { return u/d + ((u^d)>0 && u%d); }

    // Get cross point of line p, q
    // If all queries are integer, change 'Frac' to 'div'
    Frac cross(const Line &p, const Line &q) { return Frac(p.b-q.b, q.a-p.a); } // dir + : divfloor, dir - : divceil

    // min/max : ?, slope : ?, dir : ?
    struct CHT
    {
        deque<Line> V;

        // Insert line p = ax+b
        // b must be increasing (or decreasing) ('slope')
        // cross(V[i-1], V[i]) < (or >) cross(V[i], V[i+1]) ('dir')
        void push(Line p)
        {
            if(!V.empty() && V.back().a==p.a)
            {
                if(V.back().b <= p.b) return; // min : <= , max : >=
                V.pop_back();
            }
            while(V.size()>1 && !(cross(V[V.size()-2], V[V.size()-1]) < cross(V[V.size()-1], p))) V.pop_back(); // dir + : <, dir - : >
            V.push_back(p);
        }

        // Get min (or max) value at x in O(logN)
        ll query(ll x)
        {
            assert(!V.empty());
            int lo=0, hi=V.size();
            while(lo+1<hi)
            {
                int mid=lo+hi>>1;
                if(cross(V[mid-1], V[mid]) < Frac(x, 1)) lo=mid; // dir + : <, dir - : >
                else hi=mid;
            }
            return V[lo].a*x+V[lo].b;
        }

        // Get min (or max) value at x in ammortized O(1)
        // x must be increasing (or decreasing) ('dir')
        ll query2(ll x)
        {
            assert(!V.empty());
            while(V.size()>1 && cross(V[0], V[1]) < Frac(x, 1)) V.pop_front(); // dir + : <, dir - : >
            return V[0].a*x+V[0].b;
        }
    };
}