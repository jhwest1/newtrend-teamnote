mt19937 rng(1557);
int rnd() { return uniform_int_distribution<int>()(rng); }

struct Node {
  int pr, sz;
  int par, lc, rc;

  // val : value stored in node, sum : query value of subtree
  // lazy : lazy value to be applied to subtree of node (already applied to val, sum)
  ll val, sum;
  ll lazy;

  Node(ll x) {
    pr = rnd();
    sz = 1;
    par = lc = rc = 0;
    // your code goes here
    val = sum = x;
    lazy = 0;
  }

  Node() {
    pr = sz = par = lc = rc = 0;
    val = sum = 0;
    lazy = 0;
  }
};

struct Treap {
  Treap() { NS = vector<Node>(1); root = 0; }
  int newNode(ll x) { NS.push_back(Node(x)); return NS.size() - 1; }

  // NS[0] : NIL node
  int root;
  vector<Node> NS;

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

  // Merge two consecutive trees rooted at l, r
  int merge(int l, int r) {
    if (l == 0 || r == 0) return l + r;
    if (NS[l].pr > NS[r].pr) {
      prop(l);
      int t = merge(NS[l].rc, r);
      NS[t].par = l;
      NS[l].rc = t;
      recalc(l);
      return l;
    }
    else {
      prop(r);
      int t = merge(l, NS[r].lc);
      NS[t].par = r;
      NS[r].lc = t;
      recalc(r);
      return r;
    }
  }

  // Split tree rooted at node into two trees of size k, sz-k
  pii split(int node, int k) {
    if (node == 0) return {0, 0};
    assert(0 <= k && k <= NS[node].sz);
    prop(node);

    if (k <= NS[NS[node].lc].sz) {
      auto [l, r] = split(NS[node].lc, k);
      NS[node].lc = r;
      NS[r].par = node;
      NS[l].par = 0;
      recalc(node);
      return {l, node};
    }
    else {
      auto [l, r] = split(NS[node].rc, k - NS[NS[node].lc].sz - 1);
      NS[node].rc = l;
      NS[l].par = node;
      NS[r].par = 0;
      recalc(node);
      return {node, r};
    }
  }

  // ================================ Essential functions ENDS here ================================

  // Find kth node in subtree of node
  int find_kth(int node, int k) {
    prop(node);
    if (NS[NS[node].lc].sz >= k) return find_kth(NS[node].lc, k);
    if (k == NS[NS[node].lc].sz + 1) return node;
    return find_kth(NS[node].rc, k - NS[NS[node].lc].sz - 1);
  }
  // Find kth node of the tree
  int find_kth(int k) { return find_kth(root, k); }

  // Insert node x after the kth node of tree
  void insert(int k, int x) {
    assert(0 <= k && k <= NS[root].sz);
    auto [a, b] = split(root, k);
    root = merge(a, x);
    root = merge(root, b);
  }

  // Erase kth node of tree
  void erase(int k) {
    assert(1 <= k && k <= NS[root].sz);
    auto [a, d] = split(root, k - 1);
    auto [b, c] = split(d, 1);
    root = merge(a, c);
  }

  // Update val to range [l, r]
  void update(int l, int r, ll val) {
    auto [d, c] = split(root, r);
    auto [a, b] = split(d, l - 1);
    apply(b, val);
    root = merge(a, b);
    root = merge(root, c);
  }

  // Query range [l, r]
  Node query(int l, int r) {
    auto [d, c] = split(root, r);
    auto [a, b] = split(d, l - 1);
    Node ret = NS[b];
    root = merge(a, b);
    root = merge(root, c);
    return ret;
  }
};