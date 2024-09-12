#include <bits/stdc++.h>
#define pii pair<int, int>
#define pll pair<long long, long long>
#define piii pair<int, pii>
#define plll pair<long long, pll>
#define tiii array<int, 3>
#define tiiii array<int, 4>
#define ff first
#define ss second
#define ee ss.ff
#define rr ss.ss
typedef long long ll;
typedef long double ld;
const int INF = (int)1e9 + 7;

using namespace std;

const double eps = 1e-9;
void rref(vector<vector<double>> &A)
{
	int n = A.size(), m = A[0].size();
	for(int i = 0, j = 0; j < m; ++j)
	{
		int t = i;
		while(t < n && abs(A[t][j]) < eps) ++t;
		if(t == n) continue;
		swap(A[i], A[t]);
		double p = A[i][j];
		for(int k = j; k < m; ++k) A[i][k] /= p;
		for(int l = 0; l < n; ++l) if(l != i)
		{
			p = A[l][j];
			for(int k = j; k < m; ++k) A[l][k] -= p * A[i][k];
		}
		++i;
	}
}

// https://www.acmicpc.net/problem/31897
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	int A[5] = {1, 1, 1, 1, 1};
	int B[5] = {0, 1, 2, 3, 4};
	int C[5] = {0, 1, 3, 6, 10};
	int D[5];
	for(int i = 0; i < 5; ++i)
	{
		cout << A[i] << ' ' << B[i] << ' ' << C[i] << endl;
		cin >> D[i];
	}

	for(int i = 0; i < 5; ++i)
	{
		vector<vector<double>> _A;
		for(int j = 0; j < 5; ++j) if(i != j)
		{
			_A.push_back({ A[j], B[j], C[j], D[j] });
		}
		rref(_A);
		if(abs(_A[3][3]) < eps) return !(cout << round(_A[0][3]) << ' ' << round(_A[1][3]) << ' ' << round(_A[2][3]) << endl);
	}
}