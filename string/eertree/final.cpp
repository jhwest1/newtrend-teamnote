// 0-based, O(26 N)
// nd[x].pos - nd[x].len + 1 ~ nd[x].pos is palindrome for node x
// nd[S].chd[c] represents the string cSc
// nd[S].link represents the longest palindromic suffix of S
// suf[i] represents the longest palindromic suffix of S[0 ... i]
struct node {
  int link, len, par, pos;
  vector<int> chd;
  node() {
    link = len = par = pos = 0;
    chd = vector<int>(26, 1);
  }
};
vector<node> nd;
vector<int> suf;
int new_node() { nd.push_back(node()); return (int)nd.size() - 1; }
void init() { nd.clear(); suf.clear(); }
void build(string S) {
  new_node();  // node 0 represents length -1 string
  new_node();  // node 1 represents length 0 string
  nd[0].link = 0; nd[0].len = -1;
  nd[1].link = 0; nd[1].len = 0;
  for (int i = 0, x = 0; i < S.size(); i++) {
    while (x && (i == nd[x].len || S[i] != S[i - nd[x].len - 1])) x = nd[x].link;
    int c = S[i] - 'a';
    if (nd[x].chd[c] == 1) {
      int r = new_node();
      nd[r].len = nd[x].len + 2;
      int v = nd[x].link;
      while (v && (i == nd[v].len || S[i] != S[i - nd[v].len - 1])) v = nd[v].link;
      nd[r].link = nd[v].chd[c];
      nd[x].chd[c] = r;
      nd[r].par = x;
      nd[r].pos = i;
    }
    x = nd[x].chd[c];
    suf.push_back(x);
  }
}