#include <bits/stdc++.h>
using namespace std;

// S[i - P[i] ... i + P[i]] is palindrome
// manacher(S = "abcbcba") = [0, 0, 1, 3, 1, 0, 0]
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

// https://judge.yosupo.jp/problem/enumerate_palindromes
int main() {
	cin.tie(0); ios_base::sync_with_stdio(0);
	string S;
	cin >> S;

	string T;
	for (int i = 0; i < S.size(); i++) T += '#', T += S[i];
	T += '#';
	auto ret = manacher(T);
	for (int i = 1; i < (int)ret.size() - 1; i++) cout << ret[i] << ' ';
	cout << '\n';
}