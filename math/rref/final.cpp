const double eps = 1e-9;
void rref(vector<vector<double>> &A) {
  int n = A.size(), m = A[0].size();
  for (int i = 0, j = 0; j < m; ++j) {
    int t = i;
    while (t < n && abs(A[t][j]) < eps) ++t;
    if (t == n) continue;
    swap(A[i], A[t]);
    double p = A[i][j];
    for (int k = j; k < m; ++k) A[i][k] /= p;
    for (int l = 0; l < n; ++l) if (l != i) {
      p = A[l][j];
      for (int k = j; k < m; ++k) A[l][k] -= p * A[i][k];
    }
    ++i;
  }
}