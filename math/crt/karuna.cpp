typedef __int128 dll;
ll mmod(dll x, ll m) { return (x % m + m) % m; }
ll get_inv(ll a, ll b) {
	return (b == 1) ? 0 : mmod((1 - (dll)b * get_inv(b, a % b)) / (a % b), b);
}
// must satisfy 0 <= x1 < m1 and 0 <= x2 < m2 
// gcd(m1, m2) != 1 is fine
// returns pair { r, lcm(m1, m2) }
pll crt(int x1, int m1, int x2, int m2) {
	int g = __gcd(m1, m2);
	if (x1 % g != x2 % g) return {-1, 0};
	int r = x1 % g;
	m1 /= g; m2 /= g; x1 /= g; x2 /= g;
	ll m = 1ll * m1 * m2;
	ll x = (dll)x1 * m2 % m * get_inv(m2, m1) % m + (dll)x2 * m1 % m * get_inv(m1, m2) % m;
	if (x >= m) x -= m;
	return {g * x + r, g * m};
}