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

struct SplayTree {
  SplayTree() { NS = vector<Node>(1); root = 0; }
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
  // Propagates all ancestors of x
  void prop_anc(int x) {
    if (NS[x].par != 0) prop_anc(NS[x].par);
    prop(x);
  }

  // Rotate x with its parent
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
      else if (NS[NS[x].par].rc == p) NS[NS[x].par].rc = x;
    }
    recalc(p);
    recalc(x);
  }

  // Make x the root of tree
  // ammortized O(logN), should be called after consuming time to visit any internal node
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

  // Find kth node in subtree of node
  int find_kth(int node, int k) {
    assert(1 <= k && k <= NS[node].sz);
    prop(node);
    if (k <= NS[NS[node].lc].sz) return find_kth(NS[node].lc, k);
    if (k == NS[NS[node].lc].sz + 1) return node;
    return find_kth(NS[node].rc, k - NS[NS[node].lc].sz - 1);
  }
  // Find kth node of the tree, and make it root
  void find_kth(int k) { splay(find_kth(root, k)); }

  // Insert node x after the kth node in subtree of node
  void insert(int node, int k, int x) {
    if (node == 0) return;
    assert(0 <= k && k <= NS[node].sz);
    prop(node);

    if (k <= NS[NS[node].lc].sz) {
      if (NS[node].lc == 0) {
        NS[node].lc = x;
        NS[x].par = node;
      }
      else insert(NS[node].lc, k, x);
    }
    else {
      if (NS[node].rc == 0) {
        NS[node].rc = x;
        NS[x].par = node;
      }
      else insert(NS[node].rc, k - NS[NS[node].lc].sz - 1, x);
    }
    recalc(node);
  }
  // Insert node x after the kth node of tree, and make it root
  void insert(int k, int x) { insert(root, k, x); splay(x); }

  // Erase root of tree
  void erase() {
    assert(root != 0);
    prop(root);

    int p = NS[root].lc, q = NS[root].rc;
    if (p == 0 || q == 0) {
      root = p + q;
      NS[root].par = 0;
      return;
    }
    root = p;
    NS[root].par = 0;
    find_kth(NS[p].sz);
    NS[root].rc = q;
    NS[q].par = root;
    recalc(root);
  }

  // Merge [l, r]th nodes into a subtree (maybe NS[NS[root].lc].rc), and return it
  int interval(int l, int r) {
    assert(1 <= l && r <= NS[root].sz);
    int sz = NS[root].sz, ret, x;

    if (r < sz) {
      find_kth(r + 1);
      x = root;
      root = NS[x].lc;
      NS[root].par = 0;
    }

    if (l > 1) {
      find_kth(l - 1);
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
  void update(int l, int r, ll lazy) {
    assert(1 <= l && r <= NS[root].sz);
    int p = interval(l, r);
    apply(p, lazy);
    recalc(NS[p].par);
    recalc(NS[NS[p].par].par);
  }

  // Query range [l, r]
  Node query(int l, int r) {
    assert(1 <= l && r <= NS[root].sz);
    int p = interval(l, r);
    return NS[p];
  }
};