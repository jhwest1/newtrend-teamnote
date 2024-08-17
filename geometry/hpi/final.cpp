struct point { int x, y; };
point operator-(point a, point b) { return {a.x - b.x, a.y - b.y}; }
point operator+(point a, point b) { return {a.x + b.x, a.y + b.y}; }
ll operator*(point a, point b) { return 1ll * a.x * b.x + 1ll * a.y * b.y; }
ll operator/(point a, point b) { return 1ll * a.x * b.y - 1ll * a.y * b.x; }
typedef __int128_t dll;

// 0-based, O(n log n), coordinates less than 10^12
const int INF = 1e6 + 10; // 좌표범위보다 크게
struct line { point s, d;	};
bool bad(line p, line q, line r) {
  dll S = (q.s - p.s) / q.d;
  dll T = p.d / q.d;
  if (T < 0) S *= -1, T *= -1;
  p.s = p.s - r.s;
  dll x = T * p.s.x + S * p.d.x;
  dll y = T * p.s.y + S * p.d.y;
  return x * r.d.y - y * r.d.x > 0;
}

vector<line> V;
void init() {
  point P[4];
  P[0] = {-INF, -INF}; P[1] = {INF, -INF};
  P[2] = {INF, INF}; P[3] = {-INF, INF};
  V.clear();
  for (int t = 0; t < 4; t++) V.push_back({P[t], P[(t + 1) % 4] - P[t]});
}
void add_line(point s, point d) { V.push_back({s, d}); } // 시점, 벡터

vector<line> get_hpi() {
  auto sgn = [&](point a) { return a.y > 0 || (a.y == 0 && a.x > 0); };
  sort(V.begin(), V.end(), [&](auto p, auto q) { return (sgn(p.d) == sgn(q.d)) ? p.d / q.d > 0 : sgn(p.d); });

  int n = V.size();
  line dq[n];
  int r = 0, s = 0;
  for (int i = 0; i < V.size(); i++) {
    while (s - r >= 2 && bad(dq[s - 2], dq[s - 1], V[i])) --s;
    while (s - r >= 2 && bad(dq[r], dq[r + 1], V[i])) ++r;

    if (r != s && dq[s - 1].d / V[i].d == 0) {
      if (dq[s - 1].d * V[i].d > 0) { 
        if (V[i].d / (V[i].s - dq[s - 1].s) > 0) dq[s - 1] = V[i];
      }
      else return vector<line>();
    }
    else dq[s++] = V[i];

    while (s - r >= 3 && bad(dq[s - 2], dq[s - 1], dq[r])) --s;
    if (s - r >= 2 && dq[s - 2].d / dq[s - 1].d <= 0) return vector<line>();
  }
  if (s - r < 3) return vector<line>();
  return vector<line>(dq + r, dq + s);
}