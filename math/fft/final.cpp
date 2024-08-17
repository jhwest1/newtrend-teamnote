typedef complex<double> cpx;
typedef __int128 dll;
// D^2 < |X|
const int D = 32000;
const double pi = acos((double)(-1));
int M, B;
vector<int> rev;
vector<cpx> _w;
// |F|+|G| <= 2^_B
void init(int _B) {
  B = _B;
  M = 1 << B;
  _w.resize(M / 2);
  rev.resize(M);
  for (int i = 0; i < M / 2; i++) _w[i] = {cos(pi * i / M), sin(pi * i / M)};
  for (int i = 0; i < M; i++) rev[i] = (rev[i / 2] >> 1) | ((i & 1) << (B - 1));
}
void dft(vector<cpx> &F, bool inv) {
  int n = F.size();
  int b = __lg(n);
  vector<cpx> w(n);
  w[0] = w[1] = 1;
  for (int k = 2; k < n; k <<= 1) {
    for (int i = 0; i < k; ++i) w[k | i] = _w[M / k / 2 * i];
  }
  if (inv) for (int i = 0; i < n; ++i) w[i] = conj(w[i]);
  for (int i = 0; i < n; i++) {
    int p = rev[i] >> (B - b);
    if (i < p) swap(F[i], F[p]);
  }
  for (int d = 1; d < n; d *= 2) {
    for (int i = 0; i < n; i += 2 * d) {
      for (int j = 0; j < d; j++) {
        cpx b = F[i | j | d] * w[j | d];
        F[i | j | d] = F[i | j] - b;
        F[i | j] += b;
      }
    }
  }
  if (inv) for (int i = 0; i < n; i++) F[i] /= n;
}
// Safe for sum(p[i]^2 + q[i]^2) lg2(n) < 9e14
vector<dll> multiply(vector<ll> F, vector<ll> G) {
  int n = 1;
  while (n < (int)F.size() + G.size()) n <<= 1;
  vector<cpx> P(n);
  vector<cpx> Q(n);
  for (int i = 0; i < F.size(); i++) P[i] = {F[i] / D, F[i] % D};
  for (int i = 0; i < G.size(); i++) Q[i] = {G[i] / D, G[i] % D};
  dft(P, false);
  dft(Q, false);
  vector<cpx> R(n), S(n);
  for (int i = 0; i < n; i++) {
    int j = (n - i) % n;
    R[i] = (P[i] + conj(P[j])) * Q[i] * cpx(0.5, 0);
    S[i] = (P[i] - conj(P[j])) * conj(Q[j]) * cpx(0.5, 0);
  }
  dft(R, true);
  dft(S, true);
  vector<ll> Z((int)F.size() + (int)G.size() - 1);
  for (int i = 0; i < (int)Z.size(); i++) Z[i] = (ll)round(R[i].real()) * D * D + (ll)round(R[i].imag() + S[i].imag()) * D + (ll)round(S[i].real());
  return Z;
}