#include <bits/stdc++.h>
using namespace std;

// Z[i] = LCP(S[i...], S)
// get_z(S = "ababacaca") = [-, 0, 3, 0, 1, 0, 1, 0, 1]
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

// https://judge.yosupo.jp/problem/zalgorithm
int main() {
	cin.tie(0); ios_base::sync_with_stdio(0);
	string S;
	cin >> S;
	auto ret = get_z(S);
	ret[0] = S.size();
	for (int i = 0; i < (int)ret.size(); i++) cout << ret[i] << ' ';
	cout << '\n';
}