// 0-based, O(|S| + 26 * \sum|T_i|)
// v.fail = failure link of node v
// v.suf = longest suffix of node v corresponding to whole inserted string (if none, root)
// v.chd[c] = c-th children of node v in trie
// v.jmp[c] = c-th next match of node v in automaton
// call init() first, and insert strings using insert(S)
// then call calc() to calculate fail, suf, chd, jump
// AhoCorasick(S, TV) : Find occurences of TV[0], TV[1], ... in S (ending position)
// AhoCorasick(S = "mississippi", TV = [ "ss", "sis", "ippi", "pp" ]) = [ 3, 5, 6, 9, 10 ]
struct Node {
  int par, fail, suf;
  vector<int> chd, jmp;
  Node() {
    par = fail = suf = -1;
    chd = vector<int>(26, -1);
    jmp = vector<int>(26, -1);
  }
};
int root;
vector<Node> NS;
int newNode() { NS.push_back(Node()); return NS.size() - 1; }
void init() {
  NS.clear();
  root = newNode();
  NS[root].par = root;
  NS[root].fail = root;
  NS[root].suf = root;
}
void insert(const string &S) {
  int now = root;
  for (auto c : S) {
    if (NS[now].chd[c - 'a'] == -1) {
      int nxt = NS[now].chd[c - 'a'] = newNode();
      NS[nxt].par = now;
    }
    now = NS[now].chd[c - 'a'];
  }
  NS[now].suf = now;
}
void calc() {
  queue<int> Q;
  Q.push(root);
  while (!Q.empty()) {
    int now = Q.front();
    Q.pop();
    for (int i = 0; i < 26; i++) if (NS[now].chd[i] != -1) {
      int nxt = NS[now].chd[i];
      NS[now].jmp[i] = nxt;
      if (now == root) NS[nxt].fail = root;
      else {
        int p;
        for (p = NS[now].fail; p != root && NS[p].chd[i] == -1; p = NS[p].fail);
        if (NS[p].chd[i] == -1) NS[nxt].fail = root;
        else NS[nxt].fail = NS[p].chd[i];
      }
      if (NS[nxt].suf == -1) NS[nxt].suf = NS[NS[nxt].fail].suf;
      Q.push(nxt);
    }

    for (int i = 0; i < 26; i++) {
      if (NS[now].chd[i] != -1) NS[now].jmp[i] = NS[now].chd[i];
      else {
        if (now == root) NS[now].jmp[i] = now;
        else NS[now].jmp[i] = NS[NS[now].fail].jmp[i];
      }
    }
  }
}
vector<int> AhoCorasick(string S, vector<string> TV) {
  vector<int> ans;
  for (auto &T : TV) insert(T);
  calc();

  int now = root;
  for (int i = 0; i < S.size(); i++) {
    now = NS[now].jmp[S[i] - 'a'];
    // now is matching node in trie with S[0...i]
    // your code goes here
    if (NS[now].suf != root) ans.push_back(i);
  }
  return ans;
}