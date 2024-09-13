struct Node {
  int sz;
  int par, lc, rc;
  // val : value stored in node, sum : query value stored in node
  // lazy : lazy value to be applied to subtree of node (already applied to val, sum)
  ll val, sum;
  ll lazy;
  Node(ll x) {
    sz = 1;
    par = lc = rc = 0;
    // your code goes here
    val = sum = x;
    lazy = 0;
  }
  Node() {
    sz = par = lc = rc = 0;
    val = sum = 0;
    lazy = 0;
  }
};
// root must be initialized
// NS[0] : NIL node
int root;
vector<Node> NS;
void init() { NS = vector<Node>(1); root = 0; }
int newNode(ll x) { NS.push_back(Node(x)); return NS.size() - 1; }

void recalc(int node) {
  if (node == 0) return;
  int l = NS[node].lc, r = NS[node].rc;
  NS[node].sz = NS[l].sz + NS[r].sz + 1;
  // your code goes here
  NS[node].sum = NS[node].val + NS[l].sum + NS[r].sum;
}
void apply(int node, ll upd) {
  if (node == 0) return;
  // your code goes here
  NS[node].lazy += upd;
  NS[node].val += upd;
  NS[node].sum += upd * NS[node].sz;
}
void prop(int node) {
  if (node == 0) return;
  if (NS[node].lazy == 0) return;
  apply(NS[node].lc, NS[node].lazy);
  apply(NS[node].rc, NS[node].lazy);
  // your code goes here
  NS[node].lazy = 0;
}

void prop_anc(int x) { if (NS[x].par != 0) prop_anc(NS[x].par); prop(x); }
void rotate(int x) {
  assert(x != 0 && NS[x].par != 0);
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
  if (NS[x].par != 0) {
    if (NS[NS[x].par].lc == p) NS[NS[x].par].lc = x;
    else NS[NS[x].par].rc = x;
  }
  recalc(p);
  recalc(x);
}
void splay(int x) {
  root = x;
  if (x == 0) return;
  prop_anc(x);
  while (NS[x].par) {
    int p = NS[x].par, q = NS[p].par;
    if (q) rotate((NS[p].lc == x) == (NS[q].lc == p) ? p : x);
    rotate(x);
  }
}
// Find kth node in subtree of node, and make it root
int find_kth(int node, int k, int &root) {
  if (node == 0) return 0;
  assert(1 <= k && k <= NS[node].sz);
  prop(node);
  if (k <= NS[NS[node].lc].sz) return find_kth(NS[node].lc, k, root);
  if (k == NS[NS[node].lc].sz + 1) { splay(node); return root = node; }
  return find_kth(NS[node].rc, k - NS[NS[node].lc].sz - 1, root);
}
// Insert node x after the kth node in subtree of node, and make it root
void insert(int node, int k, int x, int &root) {
  assert(0 <= k && k <= NS[node].sz);
  prop(node);
  if (k <= NS[NS[node].lc].sz) {
    if (NS[node].lc == 0) {
      NS[node].lc = x;
      NS[x].par = node;
      splay(x);
      root = x;
    }
    else insert(NS[node].lc, k, x, root);
  }
  else {
    if (NS[node].rc == 0) {
      NS[node].rc = x;
      NS[x].par = node;
      splay(x);
      root = x;
    } else insert(NS[node].rc, k - NS[NS[node].lc].sz - 1, x, root);
  }
}
// Erase root of tree
void erase(int &root) {
  if (root == 0) return;
  prop(root);
  int p = NS[root].lc, q = NS[root].rc;
  if (p == 0 || q == 0) {
    root = p + q;
    NS[root].par = 0;
    return;
  }
  root = p;
  NS[root].par = 0;
  find_kth(root, NS[p].sz, root);
  NS[root].rc = q;
  NS[q].par = root;
  recalc(root);
}
// Merge [l, r]th nodes into subtree of NS[NS[root].lc].rc, and return it
int interval(int l, int r, int &root) {
  assert(1 <= l && r <= NS[root].sz);
  int sz = NS[root].sz, ret, x;
  if (r < sz) {
    find_kth(root, r + 1, root);
    x = root;
    root = NS[x].lc;
    NS[root].par = 0;
  }
  if (l > 1) {
    find_kth(root, l - 1, root);
    ret = NS[root].rc;
  }
  else ret = root;
  if (r < sz) {
    NS[root].par = x;
    NS[x].lc = root;
    root = x;
  }
  return ret;
}
// Update val to range [l, r]
void update(int l, int r, bool rev, ll lazy) {
  assert(1 <= l && r <= NS[root].sz);
  int p = interval(l, r, root);
  apply(p, lazy);
  recalc(NS[p].par);
  recalc(NS[NS[p].par].par);
}
// Query range [l, r]
Node query(int l, int r, int &root) {
  assert(1 <= l && r <= NS[root].sz);
  int p = interval(l, r, root);
  return NS[p];
}