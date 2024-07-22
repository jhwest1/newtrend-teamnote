typedef __int128 dll;
ll mmod(dll x, ll m) { return (x % m + m) % m; }
ll get_inv(ll a, ll b) {
	return (b == 1) ? 0 : mmod((1 - (dll)b * get_inv(b, a % b)) / (a % b), b);
}

