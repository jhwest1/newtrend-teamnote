// https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

const int MAXN = 5e5;

const int MOD = 998244353;
struct mint
{
    int x;
    mint() : x(0) {}
    mint(int _x) : x(_x) {}
    mint operator + (int ot) const { return x+ot>=MOD ? x+ot-MOD : x+ot; }
    mint operator - () const { return x ? MOD-x : 0; }
    mint operator - (int ot) const { return x<ot ? x+MOD-ot : x-ot; }
    mint operator * (int ot) const { return 1ll*x*ot%MOD; }
    mint operator += (int ot) { return *this = *this + ot; }
    mint operator -= (int ot) { return *this = *this - ot; }
    mint operator *= (int ot) { return *this = *this * ot; }
    operator int () const { return x; }
};

typedef pair<mint, mint> pmm;
pmm operator + (pmm p, pmm q) { return pmm(q.first*p.first, q.first*p.second+q.second); }

namespace SplayTree
{
    struct Node
    {
        int sz;
        int par, lc, rc;
        
        // val : value stored in node, sum : query value stored in node
        // lazy : lazy value to be applied to subtree of node (already applied to val, sum)
        mint val, sum;
        bool rev;
        pmm lazy;

        Node(ll x)
        {
            sz=1;
            par=lc=rc=0;

            // your code goes here
            val=sum=x;
            rev=false;
            lazy=pmm(1, 0);
        }
        Node()
        {
            sz=par=lc=rc=0;
            val=sum=0;
            rev=false;
            lazy=pmm(0, 0);
        }
    };

    int root;
    vector<Node> NS;
    void init() { NS=vector<Node>(1); root=0; }
    int newNode(ll x) { NS.push_back(Node(x)); return NS.size()-1; }

    // root must be initialized
    // NS[0] : NIL node

    // Recalculate value of node from lc, rc
    // lazy must be empty
    // changes sz, sum
    void recalc(int node)
    {
        if(node==0) return;
        int l=NS[node].lc, r=NS[node].rc;
        NS[node].sz=NS[l].sz+NS[r].sz+1;

        NS[node].sum=NS[node].val+NS[l].sum+NS[r].sum;
    }

    // Apply update value upd to subtree of node
    // update to node itself (val, sum), and to lazy value (lazy)
    // changes lazy, val, sum
    void apply(int node, bool rev, pmm lazy)
    {
        if(node==0) return;

        NS[node].rev^=rev;
        NS[node].lazy=NS[node].lazy+lazy;

        if(rev) swap(NS[node].lc, NS[node].rc);
        NS[node].val=lazy.first*NS[node].val+lazy.second;
        NS[node].sum=lazy.first*NS[node].sum+lazy.second*NS[node].sz;
    }

    // Propagate lazy value to lc, rc, and initialize lazy
    // should be called before going down to children
    // changes lazy
    void prop(int node)
    {
        if(node==0) return;

        apply(NS[node].lc, NS[node].rev, NS[node].lazy);
        apply(NS[node].rc, NS[node].rev, NS[node].lazy);
        NS[node].rev=false;
        NS[node].lazy=pmm(1, 0);
    }

    // Propagates all ancestors of x
    void prop_anc(int x)
    {
        if(NS[x].par!=0) prop_anc(NS[x].par);
        prop(x);
    }
    
    // Rotate x with its parent
    void rotate(int x)
    {
        assert(x!=0 && NS[x].par!=0);
        int p=NS[x].par, q;
        if(NS[p].lc==x)
        {
            NS[p].lc=q=NS[x].rc;
            NS[x].rc=p;
        }
        else
        {
            NS[p].rc=q=NS[x].lc;
            NS[x].lc=p;
        }
        NS[x].par=NS[p].par;
        NS[p].par=x;
        if(q) NS[q].par=p;
        if(NS[x].par!=0)
        {
            if(NS[NS[x].par].lc==p) NS[NS[x].par].lc=x;
            else NS[NS[x].par].rc=x;
        }

        recalc(p);
        recalc(x);
    }

    // Make x the root of tree
    // ammortized O(logN), should be called after consuming time to visit any internal node
    void splay(int x)
    {
        root=x;
        if(x==0) return;
        prop_anc(x);
        while(NS[x].par)
        {
            int p=NS[x].par, q=NS[p].par;
            if(q) rotate((NS[p].lc==x)==(NS[q].lc==p) ? p : x);
            rotate(x);
        }
    }

    // Find kth node in subtree of node, and make it root
    int find_kth(int node, int k, int &root)
    {
        if(node==0) return 0;
        assert(1<=k && k<=NS[node].sz);
        prop(node);
        if(k<=NS[NS[node].lc].sz) return find_kth(NS[node].lc, k, root);
        if(k==NS[NS[node].lc].sz+1) { splay(node); return root=node; }
        return find_kth(NS[node].rc, k-NS[NS[node].lc].sz-1, root);
    }

    // Insert node x after the kth node in subtree of node, and make it root
    void insert(int node, int k, int x, int &root)
    {
        assert(0<=k && k<=NS[node].sz);
        prop(node);

        if(k<=NS[NS[node].lc].sz)
        {
            if(NS[node].lc==0)
            {
                NS[node].lc=x;
                NS[x].par=node;
                splay(x);
                root=x;
            }
            else insert(NS[node].lc, k, x, root);
        }
        else
        {
            if(NS[node].rc==0)
            {
                NS[node].rc=x;
                NS[x].par=node;
                splay(x);
                root=x;
            }
            else insert(NS[node].rc, k-NS[NS[node].lc].sz-1, x, root);
        }
    }

    // Erase root of tree
    void erase(int &root)
    {
        if(root==0) return;
        prop(root);
        int p=NS[root].lc, q=NS[root].rc;
        if(p==0 || q==0)
        {
            root=p+q;
            NS[root].par=0;
            return;
        }
        root=p;
        NS[root].par=0;
        find_kth(root, NS[p].sz, root);
        NS[root].rc=q;
        NS[q].par=root;
        recalc(root);
    }

    // Merge [l, r]th nodes into subtree of NS[NS[root].lc].rc, and return it
    int interval(int l, int r, int &root)
    {
        assert(1<=l && r<=NS[root].sz);
        int sz=NS[root].sz, ret, x;

        if(r<sz)
        {
            find_kth(root, r+1, root);
            x=root;
            root=NS[x].lc;
            NS[root].par=0;
        }

        if(l>1)
        {
            find_kth(root, l-1, root);
            ret=NS[root].rc;
        }
        else ret=root;

        if(r<sz)
        {
            NS[root].par=x;
            NS[x].lc=root;
            root=x;
        }
        return ret;
    }

    // Update val to range [l, r]
    void update(int l, int r, bool rev, pmm lazy, int &root)
    {
        assert(1<=l && r<=NS[root].sz);
        int p=interval(l, r, root);
        apply(p, rev, lazy);
        recalc(NS[p].par);
        recalc(NS[NS[p].par].par);
    }
    
    // Query range [l, r]
    Node query(int l, int r, int &root)
    {
        assert(1<=l && r<=NS[root].sz);
        int p=interval(l, r, root);
        return NS[p];
    }
}

int main()
{
    int N, Q;
    scanf("%d%d", &N, &Q);
    SplayTree::init();

    for(int i=1; i<=N; i++)
    {
        int x;
        scanf("%d", &x);
        SplayTree::insert(SplayTree::root, i-1, SplayTree::newNode(x), SplayTree::root);
    }

    while(Q--)
    {
        int t;
        scanf("%d", &t);
        if(t==0)
        {
            int p, x;
            scanf("%d%d", &p, &x);
            SplayTree::insert(SplayTree::root, p, SplayTree::newNode(x), SplayTree::root);
        }
        else if(t==1)
        {
            int p;
            scanf("%d", &p); p++;
            SplayTree::find_kth(SplayTree::root, p, SplayTree::root);
            SplayTree::erase(SplayTree::root);
        }
        else if(t==2)
        {
            int l, r;
            scanf("%d%d", &l, &r); l++;
            SplayTree::update(l, r, true, pmm(1, 0), SplayTree::root);
        }
        else if(t==3)
        {
            int l, r, x, y;
            scanf("%d%d%d%d", &l, &r, &x, &y); l++;
            SplayTree::update(l, r, false, pmm(x, y), SplayTree::root);
        }
        else
        {
            int l, r;
            scanf("%d%d", &l, &r); l++;
            printf("%d\n", SplayTree::query(l, r, SplayTree::root).sum.x);
        }
    }
}