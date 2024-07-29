#include <bits/stdc++.h>
using namespace std;

void suffix_array(string S, vector<int> &sa, vector<int> &lcp) {
	int n = S.size();
	vector<int> r(n), k(n), cnt(n), ord(n);
	sa.resize(n); lcp.resize(n);
	
	iota(sa.begin(), sa.end(), 0);
	sort(sa.begin(), sa.end(), [&](int i, int j) {
		return S[i] < S[j];
	});

	r[sa[0]] = 1;
	for (int i = 1; i < n; i++) r[sa[i]] = r[sa[i - 1]] + (S[sa[i - 1]] != S[sa[i]]);

	for (int d = 1; d < n; d *= 2) {
		if (r[sa[n - 1]] == n) break;

		fill(cnt.begin(), cnt.end(), 0);
		for (int i = 0; i < n; i++) cnt[i + d < n ? r[i + d] : 0]++;
		for (int i = 1; i < n; i++) cnt[i] += cnt[i - 1];
		for (int i = 0; i < n; i++) ord[--cnt[i + d < n ? r[i + d] : 0]] = i;

		fill(cnt.begin(), cnt.end(), 0);
		for (int i = 0; i < n; i++) cnt[r[i]]++;
		for (int i = 1; i < n; i++) cnt[i] += cnt[i - 1];
		for (int i = n - 1; i >= 0; i--) sa[--cnt[r[ord[i]]]] = ord[i];

		k[sa[0]] = 1;
		for (int i = 1; i < n; i++) {
			if (r[sa[i - 1]] != r[sa[i]]) k[sa[i]] = k[sa[i - 1]] + 1;
			else {
				int p = sa[i - 1] + d < n ? r[sa[i - 1] + d] : 0;
				int q = sa[i] + d < n ? r[sa[i] + d] : 0;
				k[sa[i]] = k[sa[i - 1]] + (p != q);
			}
		}
		r = k;
	}
	for (int i = 0; i < n; i++) --r[i];
	for (int i = 0, p = 0; i < n; i++) if (r[i]) {
		int j = sa[r[i] - 1];
		while ((i + p < n ? S[i + p] : -1) == (j + p < n ? S[j + p] : -2)) ++p;
		lcp[r[i]] = p;
		p = max(p - 1, 0);
	}
}

// BOJ 9248
int main() {
	string S;
	cin >> S;
	vector<int> sa, lcp;
	suffix_array(S, sa, lcp);
	int n = S.size();
	for (int i = 0; i < n; i++) cout << sa[i] + 1 << ' ';
	cout << '\n';
	cout << "x ";
	for (int i = 1; i < n; i++) cout << lcp[i] << ' ';
	cout << '\n';
}