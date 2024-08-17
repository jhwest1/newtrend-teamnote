const int MAXN = 1010101;
bool prime[MAXN];
int phi[MAXN];

void calc() {
  fill(prime, prime + MAXN, true);
  prime[0] = prime[1] = false;

  vector<int> P;
  for (int n = 2; n < MAXN; n++) {
    if (prime[n]) {
      P.push_back(n);
      phi[n] = n - 1;
    }
    for (int pr : P) {
      ll x = (ll)n * pr;
      if (x >= MAXN) break;
      prime[x] = false;

      // for general multiplicative function, you should maintain the highest prime divisor and its order
      // mul[x] = mul[n] / mul[p ^ k] * mul[p ^ (k + 1)]

      if (n % pr == 0) {
        phi[x] = phi[n] * pr;
        break;
      } else {
        phi[x] = phi[n] * (pr - 1);
      }
    }
  }
}