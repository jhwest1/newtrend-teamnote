// S[i - P[i] ... i + P[i]] is palindrome
// manacher(S = "abcbcba") = [ 0, 0, 1, 3, 1, 0, 0 ]
vector<int> manacher(string S) {
  int n = S.size();
  vector<int> a(n);
  for (int i = 0, j = 0; i < n; i++) {
    if (j + a[j] > i) a[i] = min(a[2 * j - i], j + a[j] - i);
    while (i + a[i] < n && i >= a[i] && S[i + a[i]] == S[i - a[i]]) ++a[i];
    if (i + a[i] > j + a[j]) j = i;
  }
  for (int i = 0; i < n; i++) --a[i];
  return a;
}
// Z[i] = LCP(S[i...], S)
// get_z(S = "ababacaca") = [ -, 0, 3, 0, 1, 0, 1, 0, 1 ]
vector<int> get_z(string S) {
  int n = S.size();
  vector<int> a(n);
  for (int i = 1, j = 0; i < n; i++) {
    if (j + a[j] > i) a[i] = min(j + a[j] - i, a[i - j]);
    while (i + a[i] < n && S[a[i]] == S[i + a[i]]) ++a[i];
    if (i + a[i] > j + a[j]) j = i;
  }
  return a;
}