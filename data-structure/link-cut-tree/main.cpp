struct Node {
  int par, lc, rc, lc2, rc2, pt;
  ll val;
  int hsz, lsz;
  ll hsum, lsum;
  ll hlazy, llazy;
  bool rev;
  Node(ll x) {
    par = lc = rc = lc2 = rc2 = pt = 0;
    val = x;
    hsz = 1; lsz = 0;
    hsum = x; lsum = 0;
    hlazy = llazy = 0;
    rev = false;
  }
  Node() {
    par = lc = rc = lc2 = rc2 = pt = 0;
    val = 0;
    hsz = lsz = 0;
    hsum = lsum = 0;
    hlazy = llazy = 0;
    rev = false;
  }
};
Node NS[MAXN + 10];

void recalc(int node) {
  if (node == 0) return;
  assert(NS[node].hlazy == 0 && NS[node].llazy == 0 && !NS[node].rev);
  int l = NS[node].lc, r = NS[node].rc, l2 = NS[node].lc2, r2 = NS[node].rc2, p = NS[node].pt;
  NS[node].hsz = NS[l].hsz + NS[r].hsz + 1;
  NS[node].lsz = NS[l].lsz + NS[r].lsz + (NS[l2].hsz + NS[l2].lsz) + (NS[r2].hsz + NS[r2].lsz) + (NS[p].hsz + NS[p].lsz);
  NS[node].hsum = NS[l].hsum + NS[r].hsum + NS[node].val;
  NS[node].lsum = NS[l].lsum + NS[r].lsum + (NS[l2].hsum + NS[l2].lsum) + (NS[r2].hsum + NS[r2].lsum) + (NS[p].hsum + NS[p].lsum);
}
void apply(int node, ll hupd, ll lupd, bool rev) {
  if (node == 0) return;
  NS[node].rev ^= rev;
  NS[node].hlazy += hupd;
  NS[node].llazy += lupd;
  if (rev) swap(NS[node].lc, NS[node].rc);
  NS[node].val += hupd;
  NS[node].hsum += hupd * NS[node].hsz;
  if (NS[node].lsz) NS[node].lsum += lupd * NS[node].lsz;
}
void prop(int node) {
  if (node == 0) return;
  apply(NS[node].lc, NS[node].hlazy, NS[node].llazy, NS[node].rev);
  apply(NS[node].rc, NS[node].hlazy, NS[node].llazy, NS[node].rev);
  apply(NS[node].lc2, NS[node].llazy, NS[node].llazy, false);
  apply(NS[node].rc2, NS[node].llazy, NS[node].llazy, false);
  apply(NS[node].pt, NS[node].llazy, NS[node].llazy, false);
  NS[node].hlazy = NS[node].llazy = 0;
  NS[node].rev = false;
}

bool isRoot(int x) { return (NS[x].par == 0 || (NS[NS[x].par].lc != x && NS[NS[x].par].rc != x)); }
bool isRoot2(int x) { return (NS[x].par == 0 || NS[NS[x].par].pt == x); }
void prop_anc(int x) { if (!isRoot(x)) prop_anc(NS[x].par); prop(x); }
void prop_anc2(int x) { if (!isRoot2(x)) prop_anc2(NS[x].par); prop(x); }
void prop_anc3(int x) { if (NS[x].par) prop_anc3(NS[x].par); prop(x); }
void rotate(int x) {
  assert(x != 0 && !isRoot(x));
  int p = NS[x].par, q;
  if (NS[p].lc == x) {
    NS[p].lc = q = NS[x].rc;
    NS[x].rc = p;
  }
  else {
    NS[p].rc = q = NS[x].lc;
    NS[x].lc = p;
  }
  NS[x].par = NS[p].par;
  NS[p].par = x;
  if (q) NS[q].par = p;
  if (NS[x].par) {
    if (NS[NS[x].par].lc == p) NS[NS[x].par].lc = x;
    else if (NS[NS[x].par].rc == p) NS[NS[x].par].rc = x;
    else if (NS[NS[x].par].lc2 == p) NS[NS[x].par].lc2 = x;
    else if (NS[NS[x].par].rc2 == p) NS[NS[x].par].rc2 = x;
    else if (NS[NS[x].par].pt == p) NS[NS[x].par].pt = x;
  }
  recalc(p);
  recalc(x);
}
void rotate2(int x) {
  assert(x != 0 && !isRoot2(x));
  int p = NS[x].par, q;
  if (NS[p].lc2 == x) {
    NS[p].lc2 = q = NS[x].rc2;
    NS[x].rc2 = p;
  }
  else {
    NS[p].rc2 = q = NS[x].lc2;
    NS[x].lc2 = p;
  }
  NS[x].par = NS[p].par;
  NS[p].par = x;
  if (q) NS[q].par = p;
  if (NS[x].par) {
    if (NS[NS[x].par].lc == p) NS[NS[x].par].lc = x;
    else if (NS[NS[x].par].rc == p) NS[NS[x].par].rc = x;
    else if (NS[NS[x].par].lc2 == p) NS[NS[x].par].lc2 = x;
    else if (NS[NS[x].par].rc2 == p) NS[NS[x].par].rc2 = x;
    else if (NS[NS[x].par].pt == p) NS[NS[x].par].pt = x;
  }
  recalc(p);
  recalc(x);
}
void splay(int x) {
  if (x == 0) return;
  prop_anc(x);
  int root = x;
  while (!isRoot(root)) root = NS[root].par;
  int l2 = NS[root].lc2, r2 = NS[root].rc2;
  NS[root].lc2 = NS[root].rc2 = 0;
  while (!isRoot(x)) {
    int p = NS[x].par, q = NS[p].par;
    if (!isRoot(p)) rotate((NS[p].lc == x) == (NS[q].lc == p) ? p : x);
    rotate(x);
  }
  NS[x].lc2 = l2;
  NS[x].rc2 = r2;
  recalc(x);
}
void splay2(int x) {
  if (x == 0) return;
  prop_anc2(x);
  while (!isRoot2(x)) {
    int p = NS[x].par, q = NS[p].par;
    if (!isRoot2(p)) rotate2((NS[p].lc2 == x) == (NS[q].lc2 == p) ? p : x);
    rotate2(x);
  }
}
int find_right(int x) {
  assert(isRoot2(x));
  while (NS[x].rc2 != 0) {
    prop(x);
    x = NS[x].rc2;
  }
  splay2(x);
  return x;
}
void erase(int x) {
  assert(isRoot2(x));
  prop(x);
  int p = NS[x].lc2, q = NS[x].rc2;
  NS[x].lc2 = 0;
  NS[x].rc2 = 0;
  recalc(x);
  if (p == 0 || q == 0) {
    int root = p + q;
    if (root) NS[root].par = NS[x].par;
    if (NS[x].par) NS[NS[x].par].pt = root;
    recalc(NS[root].par);
    return;
  }
  int root = p;
  NS[root].par = 0;
  root = find_right(root);
  NS[root].rc2 = q;
  NS[q].par = root;
  NS[root].par = NS[x].par;
  if (NS[x].par) NS[NS[x].par].pt = root;
  recalc(root);
  recalc(NS[root].par);
}
void insert(int x, int y) {
  prop(x);
  if (NS[x].pt == 0) {
    NS[x].pt = y;
    NS[y].par = x;
  } else {
    int x2 = NS[x].pt;
    x2 = find_right(x2);
    NS[x2].rc2 = y;
    NS[y].par = x2;
    recalc(x2);
  }
  recalc(x);
}
int access(int x) {
  prop_anc3(x);
  splay(x);
  if (NS[x].rc != 0) insert(x, NS[x].rc);
  NS[x].rc = 0;
  recalc(x);
  int ret = x;
  while (NS[x].par) {
    splay2(x);
    int y = NS[x].par;
    ret = y;
    splay(y);
    erase(x);
    if (NS[y].rc != 0) insert(y, NS[y].rc);
    NS[y].rc = x;
    NS[x].par = y;
    recalc(x);
    recalc(y);
    splay(x);
  }
  prop(x);
  return ret;
}
void link(int x, int p) {
  access(x);
  access(p);
  NS[x].lc = p;
  NS[p].par = x;
  recalc(x);
}
void cut(int x) {
  access(x);
  NS[NS[x].lc].par = 0;
  NS[x].lc = 0;
  recalc(x);
}
int lca(int x, int y) {
  access(x);
  return access(y);
}
void reroot(int x) {
  access(x);
  apply(x, 0, 0, true);
}
int findRoot(int x) {
  access(x);
  while (NS[x].lc) x = NS[x].lc;
  splay(x);
  return x;
}
int findParent(int x) {
  access(x);
  x = NS[x].lc;
  if (x == 0) return 0;
  while (NS[x].rc) x = NS[x].rc;
  splay(x);
  return x;
}
int depth(int x) {
  access(x);
  return NS[NS[x].lc].hsz + 1;
}
//=========================================
ll subtree_query(int r, int v) {
  reroot(r);
  access(v);
  int p = NS[v].pt;
  return NS[v].val + NS[p].hsum + NS[p].lsum;
}
void subtree_update(int r, int v, ll k) {
  reroot(r);
  access(v);
  apply(NS[v].pt, k, k, false);
  NS[v].val += k;
  recalc(v);
}
ll path_query(int u, int v) {
  reroot(u);
  access(v);
  return NS[v].hsum;
}
void path_update(int u, int v, ll k) {
  reroot(u);
  access(v);
  apply(v, k, 0, false);
}