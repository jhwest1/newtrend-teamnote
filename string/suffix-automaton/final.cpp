// 0-based, O(26 * |S|)
// v.link = suffix link of v
// v.len = length of longest substring corresponding to v
// v.pos = one element of endpos(v)
// v.chd[c] = c-th children node of v in suffix automaton
// e.x = child node of edge e in suffix tree
// [e.l, e.r] = substring of S corresponding to edge e in suffix tree
// call init() first, and build suffix automaton by calling build(S)
// call init() first, and build suffix tree by calling suffix_tree(S)
// all nodes in nd are used in both suffix automaton and suffix tree
const int CH = 26;
struct node {
  int link, len, pos;
  vector<int> chd;
  bool flag;
  node() {
    link = len = pos = 0;
    flag = false;
    chd = vector<int>(CH);
  }
};
vector<node> nd;
void init() { nd.clear(); }
int new_node() {
  nd.push_back(node());
  return (int)nd.size() - 1;
}
void build(string S) {
  new_node();
  nd[0].link = -1;
  int prv = 0;
  for (int i = 0; i < S.size(); i++) {
    int c = S[i] - 'a';
    int cur = new_node();
    nd[cur].len = nd[prv].len + 1;
    nd[cur].pos = i;
    nd[cur].flag = true;
    for (; prv != -1 && !nd[prv].chd[c]; prv = nd[prv].link) nd[prv].chd[c] = cur;
    if (prv != -1) {
      int x = nd[prv].chd[c];
      if (nd[x].len == nd[prv].len + 1) nd[cur].link = x;
      else {
        int y = new_node();
        nd[y].len = nd[prv].len + 1;
        nd[y].pos = i;
        nd[y].link = nd[x].link;
        nd[y].chd = nd[x].chd;
        nd[x].link = nd[cur].link = y;
        for (; prv != -1 && nd[prv].chd[c] == x; prv = nd[prv].link) nd[prv].chd[c] = y;
      }
    }
    prv = cur;
  }
}
struct edge { int x, l, r; };
vector<vector<edge>> g;
void suffix_tree(string S) {
  reverse(S.begin(), S.end());
  build(S);
  reverse(S.begin(), S.end());
  int n = nd.size();
  g.resize(n);
  for (int v = 0; v < n; v++) nd[v].pos = (int)S.size() - 1 - nd[v].pos;
  for (int v = 1; v < n; v++) {
    int x = nd[v].link;
    int l = nd[v].pos + nd[x].len;
    int r = nd[v].pos + nd[v].len - 1;
    g[x].push_back({v, l, r});
  }
  for (int v = 0; v < n; v++) {
    sort(g[v].begin(), g[v].end(), [&](edge e, edge f) { return S[e.l] < S[f.l]; });
  }
}
void dfs(int v, vector<int> &sa) {
  if (nd[v].flag) sa.push_back(nd[v].pos);
  for (auto [x, l, r] : g[v]) dfs(x, sa);
}
void suffix_array(string S, vector<int> &sa, vector<int> &lcp) {
  suffix_tree(S);
  dfs(0, sa);
  int n = S.size();
  lcp.resize(n);
  vector<int> r(n);
  for (int i = 0; i < n; i++) r[sa[i]] = i;
  for (int i = 0, p = 0; i < n; i++) if (r[i]) {
    int j = sa[r[i] - 1];
    while ((i + p < n ? S[i + p] : -1) == (j + p < n ? S[j + p] : -2)) ++p;
    lcp[r[i]] = p;
    p = max(p - 1, 0);
  }
}