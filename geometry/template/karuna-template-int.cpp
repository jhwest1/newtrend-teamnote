#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
struct point {
	int x, y;
};
point operator+(point a, point b) { return {a.x + b.x, a.y + b.y}; }
point operator-(point a, point b) { return {a.x - b.x, a.y - b.y}; }
ll operator*(point a, point b) { return 1ll * a.x * b.x + 1ll * a.y * b.y; }
ll operator/(point a, point b) { return 1ll * a.x * b.y - 1ll * a.y * b.x; }

int ccw(point p, point q, point r) {
	ll x = (q - p) / (r - p);
	return (x > 0) - (x < 0);
}
bool point_on_line(point p, point q, point r) {
	ll x = (r - p) * (q - p);
	return ccw(p, q, r) == 0 && 0 <= x && x <= (q - p) * (q - p);
}
// 교점이 없으면 0, 교점이 선분의 끝점이면 1, 두 선분이 교차하면 2, 무수히 많으면 3
int line_intersect_verbose(point p, point q, point r, point s) {
	if (ccw(p, q, r) == 0 && ccw(p, q, s) == 0) {
		ll u = (r - p) * (q - p);
		ll v = (s - p) * (q - p);
		ll l = (q - p) * (q - p);
		if (u > v) swap(u, v);
		// intersection of [0, l] and [u, v]
		if (l < u || v < 0) return 0;
		else if (l == u || v == 0) return 1;
		else return 3;
	}
	if (point_on_line(r, s, p) || point_on_line(r, s, q)) return 1;
	if (point_on_line(p, q, r) || point_on_line(p, q, s)) return 1;
	return (ccw(p, q, r) * ccw(p, q, s) < 0 && ccw(r, s, p) * ccw(r, s, q) < 0) ? 2 : 0;
}

// https://www.acmicpc.net/problem/27718
int main() {
	cin.tie(0); ios_base::sync_with_stdio(0);
	int n;
	cin >> n;
	point s[n], e[n];
	for (int i = 0; i < n; i++) {
		cin >> s[i].x >> s[i].y >> e[i].x >> e[i].y;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << line_intersect_verbose(s[i], e[i], s[j], e[j]);
		}
		cout << '\n';
	}
}