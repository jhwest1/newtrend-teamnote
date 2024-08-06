const int MOD = 998244353;
const mint G = 3; // primitive root
// or (MOD, G) = (0xffffffff00000001, 7)
// define mpow here
void dft(vector<mint> &F, bool inv) {
	int n = F.size();
	int b = __lg(n);
	vector<int> rev(n);
	for (int i = 0; i < n; i++) {
		rev[i] = (rev[i / 2] >> 1) | ((i & 1) << (b - 1));
		if(i < rev[i]) swap(F[i], F[rev[i]]);
	}
	for (int d = 1; d < n; d <<= 1) {
		mint w = mpow(G, inv ? MOD - 1 - (MOD - 1) / (2 * d) : (MOD - 1) / (2 * d));
		for (int i = 0; i < n; i += 2 * d) {
			mint r = 1;
			for (int j = 0; j < d; j++) {
				mint b = F[i|j|d] * r;
				F[i|j|d] = F[i|j] - b;
				F[i|j] += b;
				r *= w;
			}
		}
	}
	if (inv) {
		mint val = mpow(n, MOD - 2);
		for (int i = 0; i < n; i++) F[i] *= val;
	}
}
vector<mint> multiply(vector<mint> F, vector<mint> G) {
	int n = 1, m = (int)F.size() + (int)G.size() - 1;
	while (n <= m) n <<= 1;
	F.resize(n); G.resize(n);
	dft(F, false); dft(G, false);
	for (int i = 0; i < n; i++) F[i] *= G[i];
	dft(F, true);
	F.resize(m);
	return F;
}