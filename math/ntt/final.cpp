const int MOD = 998244353;
const mint G = 3;  // primitive root
// or (MOD, G) = (0xffffffff00000001, 7)
// define mint, mpow
void dft(vector<mint> &F, bool inv) {
  int n = F.size();
  int b = __lg(n);
  vector<int> rev(n);
  for (int i = 0; i < n; i++) {
    rev[i] = (rev[i / 2] >> 1) | ((i & 1) << (b - 1));
    if (i < rev[i]) swap(F[i], F[rev[i]]);
  }
  vector<mint> w(n);
  w[0] = w[1] = 1;
  for (int k = 2; k < n; k <<= 1) {
    mint z[2] = {1, mpow(G, inv ? MOD - 1 - (MOD - 1) / k / 2 : (MOD - 1) / k / 2)};
    for (int i = k; i < (k << 1); ++i) w[i] = w[i >> 1] * z[i & 1];
  }
  for (int d = 1; d < n; d <<= 1) {
    for (int i = 0; i < n; i += 2 * d) {
      for (int j = 0; j < d; j++) {
        mint b = F[i | j | d] * w[j | d];
        F[i | j | d] = F[i | j] - b;
        F[i | j] += b;
      }
    }
  }
  if (inv) {
    mint val = mpow(n, MOD - 2);
    for (int i = 0; i < n; i++) F[i] *= val;
  }
}
vector<mint> multiply(vector<mint> F, vector<mint> G) {
  int n = 1;
  int sz = F.size() + G.size() - 1;
  while (n < (int)F.size() + G.size()) n <<= 1;
  F.resize(n); G.resize(n);
  dft(F, false); dft(G, false);
  for (int i = 0; i < n; i++) F[i] *= G[i];
  dft(F, true);
  F.resize(sz);
  return F;
}