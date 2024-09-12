#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

// 0-based, O(n log n)
// S   = "abcababc"
// sa  = [ 3, 5, 0, 4, 6, 1, 7, 2 ]
// lcp = [ -, 2, 3, 0, 1, 2, 0, 1 ]
void suffix_array(string S, vector<int> &sa, vector<int> &lcp) {
  int n = S.size();
  vector<int> r(n), k(n), cnt(n), ord(n);
  sa.resize(n);
  lcp.resize(n);

  iota(sa.begin(), sa.end(), 0);
  sort(sa.begin(), sa.end(), [&](int i, int j) { return S[i] < S[j]; });

  r[sa[0]] = 1;
  for (int i = 1; i < n; i++) r[sa[i]] = r[sa[i - 1]] + (S[sa[i - 1]] != S[sa[i]]);

  for (int d = 1; d < n; d *= 2) {
    if (r[sa[n - 1]] == n) break;

    fill(cnt.begin(), cnt.end(), 0);
    for (int i = 0; i < n; i++) cnt[i + d < n ? r[i + d] : 0]++;
    for (int i = 1; i < n; i++) cnt[i] += cnt[i - 1];
    for (int i = 0; i < n; i++) ord[--cnt[i + d < n ? r[i + d] : 0]] = i;

    fill(cnt.begin(), cnt.end(), 0);
    for (int i = 0; i < n; i++) cnt[r[i]]++;
    for (int i = 1; i < n; i++) cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; i--) sa[--cnt[r[ord[i]]]] = ord[i];

    k[sa[0]] = 1;
    for (int i = 1; i < n; i++) {
      if (r[sa[i - 1]] != r[sa[i]]) k[sa[i]] = k[sa[i - 1]] + 1;
      else {
        int p = sa[i - 1] + d < n ? r[sa[i - 1] + d] : 0;
        int q = sa[i] + d < n ? r[sa[i] + d] : 0;
        k[sa[i]] = k[sa[i - 1]] + (p != q);
      }
    }
    r = k;
  }
  for (int i = 0; i < n; i++) --r[i];
  for (int i = 0, p = 0; i < n; i++) if (r[i]) {
    int j = sa[r[i] - 1];
    while ((i + p < n ? S[i + p] : -1) == (j + p < n ? S[j + p] : -2)) ++p;
    lcp[r[i]] = p;
    p = max(p - 1, 0);
  }
}

int N;
string S;
vector<int> sa, lcp, r;
vector<vector<int>> sp;
void init(string _S)
{
    S=_S; N=S.size(); r=vector<int>(N);
    suffix_array(S, sa, lcp);
    for(int i=0; i<N; i++) r[sa[i]]=i;
    sp=vector<vector<int>>(__lg(N)+1, vector<int>(N));
    sp[0]=lcp;
    for(int i=1; i<sp.size(); i++) for(int j=0; j<N; j++) sp[i][j]=min(sp[i-1][j], sp[i-1][min(N-1, j+(1<<(i-1)))]);
}
int get_lcp(int a, int b)
{
    if(a==b) return N-a;
    a=r[a]; b=r[b];
    if(a>b) swap(a, b);
    int len=b-a, k=__lg(len);
    return min(sp[k][a+1], sp[k][b-(1<<k)+1]);
}
int compare(int l1, int r1, int l2, int r2)
{
    r1=max(r1, l1-1); r2=max(r2, l2-1);
    int len=min(r1-l1+1, r2-l2+1), x=get_lcp(l1, l2);
    if(x<len) return (S[l1+x]>S[l2+x])-(S[l1+x]<S[l2+x]);
    else return (r1-l1>r2-l2)-(r1-l1<r2-l2);
}

mt19937 rng(time(NULL));
int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

int main()
{
    // ios_base::sync_with_stdio(false); cin.tie(NULL);
    
    int N=200;
    string S;
    for(int i=0; i<N; i++) S+='a'+rnd(0, 5);
    cout << S << "\n";
    // cin >> S;
    init(S);
    for(int l1=0; l1<S.size(); l1++) for(int r1=0; r1<S.size(); r1++) for(int l2=0; l2<S.size(); l2++) for(int r2=0; r2<S.size(); r2++)
    {
        // cin >> l1 >> r1 >> l2 >> r2;
        int t=compare(l1, r1, l2, r2);
        string P1, P2;
        if(l1<=r1) P1=string(S.begin()+l1, S.begin()+r1+1);
        if(l2<=r2) P2=string(S.begin()+l2, S.begin()+r2+1);
        int t2=P1.compare(P2);
        t2=(t2>0)-(t2<0);
        assert(t==t2);
        // cout << l1 << " " << r1 << " " << l2 << " " << r2 << "\n";
        // cout << t << " " << t2 << "\n";
    }
}