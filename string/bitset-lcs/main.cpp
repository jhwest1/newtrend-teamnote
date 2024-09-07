// O(NM/64)
#define get(arr, x) (((arr[x >> 6] >> (x & 63)) & 1) == 1)
#define set(arr, x) (arr[x >> 6] |= 1llu << (x & 63))
using ull = unsigned long long;
// return lcs
int lcs(string A, string B) {
  int N = A.size(), M = B.size();
  int sz = (M >> 6) + 1;

  vector<ull> S[256];
  for(int c = 0; c < 256; c++) S[c].resize(sz);
  for(int j = 0; j < M; j++) set(S[B[j]], j);  
  vector<ull> row(sz);
  for(int j = 0; j < M; j++) if(A[0] == B[j]) { set(row, j); break; }

  for(int i = 1; i < N; i++) {
    ull shl_carry = 1;
    ull minus_carry = 0;
    for(int k = 0; k < sz; k++) {
      // Compute k-th block of x == S[A[i]] OR D[i-1]
      ull x_k = S[A[i]][k] | row[k];
      // Compute k-th block of "(D[i-1] << 1) | 1"
      ull term_1 = (row[k] << 1) | shl_carry;
      shl_carry = row[k] >> 63;
      // Compute k-th block of "x - ((D[i-1] << 1) | 1)"
      auto sub_carry = [](ull &x, ull y){
        ull tmp = x;
        return (x = tmp - y) > tmp;
      };
      ull term_2 = x_k;
      minus_carry = sub_carry(term_2, minus_carry);
      minus_carry += sub_carry(term_2, term_1);
      row[k] = x_k & (x_k ^ term_2);
    }
    row[M >> 6] &= (1llu << (M & 63)) - 1;
  }
  int ret = 0;
  for(int j = 0; j < M; j++) if(get(row, j)) ret += 1;
  return ret;
}