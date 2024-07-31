const int MOD = 998244353;
const mint G = 3; // primitive root
// or (MOD, G) = (0xffffffff00000001, 7)
// define mpow here
int M, B;
vector<int> rev;
vector<mint> w;
void init(int _B) {
	B = _B;
	M = 1 << B;
	w.resize(2 * M + 1);
	rev.resize(M);
	mint g = mpow(G, (MOD - 1) >> (B + 1));
	w[0] = 1;
	for (int i = 1; i <= 2 * M; i++) w[i] = w[i - 1] * g;
	for (int i = 0; i < M; i++) {
		rev[i] = (rev[i / 2] >> 1) | ((i & 1) << (B - 1));
	}
}
void dft(vector<mint> &F, bool inv) {
	int n = F.size();
	int b = __lg(n);
	for (int i = 0; i < n; i++) {
		int p = rev[i] >> (B - b);
		if (i < p) swap(F[i], F[p]);
	}
	for (int d = 1; d < n; d <<= 1) {
		for (int i = 0; i < n; i += 2 * d) {
			for (int j = 0; j < d; j++) {
				mint r = inv ? w[2 * M - M / d * j] : w[M / d * j];
				mint a = F[i + j];
				mint b = F[i + j + d] * r;
				F[i + j] = a + b;
				F[i + j + d] = a - b;
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
	while (n < (int)F.size() + G.size()) n <<= 1;
	F.resize(n); G.resize(n);
	dft(F, false); dft(G, false);
	for (int i = 0; i < n; i++) F[i] *= G[i];
	dft(F, true);
	return F;
}