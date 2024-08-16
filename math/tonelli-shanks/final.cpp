// find one x such that x^2 = n (mod p)
// returns -1 if does not exist
// p should be a prime, 0 <= n < p
// O(log^2 p)
mt19937 rnd(1557);
ll rng(ll l, ll r) { return uniform_int_distribution<ll>(l, r)(rnd); }
ll mpow(ll a, ll x, ll mod) {
  ll r = 1;
  while (x) {
    if (x & 1) r = r * a % mod;
    a = a * a % mod;
    x /= 2;
  }
  return r;
}
ll quadratic_residue(ll n, ll p) {
  if (n == 0) return 0;
  if (p == 2) return n;
  if (mpow(n, (p - 1) / 2, p) != 1) return -1;

  int q = p - 1, s = __builtin_ctz(q);
  q >>= s;

  ll x = mpow(n, (q + 1) / 2, p);
  ll t = mpow(n, q, p);
  ll z = rng(1, p - 1);
  while (mpow(z, (p - 1) / 2, p) == 1) z = rng(1, p - 1);

  while (t != 1) {
    ll i = 0, v = t;
    while (v != 1) v = v * v % p, ++i;
    ll b = mpow(z, q * (1ll << (s - i - 1)), p);
    x = x * b % p;
    t = t * b % p * b % p;
  }
  return x;
}