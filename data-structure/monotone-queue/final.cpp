const int MAXN = 5e5;
int N;
// dp[i] : optimal dp value of i
// cnt[i] : how much transitions were used in optimal dp[i]
// memo[i] : previous transition position from i
// V : restored optimal solution
ll dp[MAXN + 10];
int cnt[MAXN + 10], memo[MAXN + 10];
vector<int> V;
void init(int _N) { N = _N; }
// cost(p, q) must be monge
// cost(a, d) + cost(b, c) > cost(a, c) + cost(b, d)
ll cost(int p, int q) {}
// Get maximum x that dp[p]+cost(p, x) < dp[q]+cost(q, x) (p is optimal than q)
// p < q must hold
int cross(int p, int q) {
  int lo = q, hi = N + 1;
  while (lo + 1 < hi) {
    int mid = lo + hi >> 1;
    if (dp[p] + cost(p, mid) < dp[q] + cost(q, mid)) lo = mid;  // min : <, max : >
    else hi = mid;
  }
  return lo;
}
// Get optimal dp[N]
// dp[i] = min(or max)_{j<i} (dp[j] + cost(j, i))
// changes dp, cnt, memo, V
void solve() {
  // initialize dp, cnt, memo, V, (other data structures)
  for (int i = 0; i <= N; i++) dp[i] = cnt[i] = memo[i] = 0;
  V.clear();
  deque<int> DQ;
  DQ.push_back(0);
  for (int i = 1; i <= N; i++) {
    while (DQ.size() > 1 && dp[DQ[0]] + cost(DQ[0], i) >= dp[DQ[1]] + cost(DQ[1], i)) DQ.pop_front();  // min : >=, max : <=
    // opt = argmin(or max)_{j<i} (dp[j] + cost(j, i))
    int opt = DQ.front();
    dp[i] = dp[opt] + cost(opt, i);
    cnt[i] = cnt[opt] + 1;
    memo[i] = opt;
    // cross(DQ[i-1], DQ[i]) < cross(DQ[i], DQ[i+1])
    while (DQ.size() > 1 && !(cross(DQ[DQ.size() - 2], DQ[DQ.size() - 1]) < cross(DQ[DQ.size() - 1], i))) DQ.pop_back();
    DQ.push_back(i);
  }
  for (int i = N; i > 0;) {
    V.push_back(i);
    i = memo[i];
  }
  V.push_back(0);
  reverse(V.begin(), V.end());
}