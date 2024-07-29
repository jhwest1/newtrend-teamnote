#include <bits/stdc++.h>
#define ff first
#define ss second
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;

int main() {
	int n;
	cin >> n;
	int x[n], y[n]; // 중복점 절대 안됨
	for (int i = 0; i < n; i++) cin >> x[i] >> y[i];

	int ord[n], rnk[n];
	iota(ord, ord + n, 0);
	sort(ord, ord + n, [&](int i, int j) {
		return x[i] == x[j] ? y[i] < y[j] : x[i] < x[j];
	});
	for (int i = 0; i < n; i++) rnk[ord[i]] = i;

	vector<pii> bdz;
	for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) bdz.push_back({ord[i], ord[j]});
	auto det = [&](pii l, pii r) {
		return 1ll * (x[l.ss] - x[l.ff]) * (y[r.ss] - y[r.ff]) - 1ll * (y[l.ss] - y[l.ff]) * (x[r.ss] - x[r.ff]);
	};
	sort(bdz.begin(), bdz.end(), [&](pii l, pii r) {
		ll x = det(l, r);
		if (x == 0) {
			return rnk[l.ff] == rnk[r.ff] ? rnk[l.ss] < rnk[r.ss] : rnk[l.ff] < rnk[r.ff];
		}
		else return x > 0;
	});

	for (int i = 0, p = 0; i < bdz.size(); i = p) {
		while (p < bdz.size() && det(bdz[i], bdz[p]) == 0) {
			++p;
		}
		for (int j = i; j < p; j++) {
			auto [u, v] = bdz[j];
			assert(rnk[v] == rnk[u] + 1);
			// your code goes here
			
			swap(rnk[u], rnk[v]);
			swap(ord[rnk[u]], ord[rnk[v]]);
		}
	}
}