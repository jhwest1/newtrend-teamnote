#include <bits/stdc++.h>
using namespace std;

vector<int> manacher(string S) {
	int n = S.size();
	vector<int> a(n);
	for (int i = 0, j = 0; i < n; i++) {
		if (j + a[j] > i) a[i] = min(a[2 * j - i], j + a[j] - i);
		while (i + a[i] < n && i >= a[i] && S[i + a[i]] == S[i - a[i]]) ++a[i];
		if (i + a[i] > j + a[j]) j = i;
	}
	return a;
}
