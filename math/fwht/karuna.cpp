#include <bits/stdc++.h>
using namespace std;

// it's just n-dimensional fft
void xor_dft(vector<int> &a, bool inv) {
	int n = __lg(a.size());
	for (int i = 0; i < n; i++) {
		for (int msk = 0; msk < (1 << n); msk++) {
			if (msk >> i & 1) {
				int x = a[msk ^ (1 << i)];
				int y = a[msk];
				a[msk ^ (1 << i)] = x + y;
				a[msk] = x - y;
			}
		}
		if (inv) for (int msk = 0; msk < (1 << n); msk++) a[msk] /= 2;
	}
}
// it's just n-dimensional prefix sum
void or_dft(vector<int> &a, bool inv) {
	int n = __lg(a.size());
	for (int i = 0; i < n; i++) {
		for (int msk = 0; msk < (1 << n); msk++) {
			if (msk >> i & 1) {
				if (!inv) a[msk] += a[msk ^ (1 << i)];
				else a[msk] -= a[msk ^ (1 << i)];
			}
		}
	}
}
// it's just n-dimensional suffix sum 
void and_dft(vector<int> &a, bool inv) {
	int n = __lg(a.size());
	for (int i = 0; i < n; i++) {
		for (int msk = 0; msk < (1 << n); msk++) {
			if (msk >> i & 1) {
				if (!inv) a[msk ^ (1 << i)] += a[msk];
				else a[msk ^ (1 << i)] -= a[msk];
			}
		}
	}
}