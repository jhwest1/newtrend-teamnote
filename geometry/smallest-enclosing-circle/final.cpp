/* include 5 basic operators, ccw, intersect from double template */
point rot(point p) { return {-p.y, p.x}; }
double dst(point p, point q) { return sqrt((p - q) * (p - q)); }
point center(point p, point q, point r) {
  point x = 0.5 * (p + q);
  point y = 0.5 * (q + r);
  return intersect(x, rot(q - p), y, rot(r - q));
}
// O(n) randomized
pair<point, double> min_enclosing_circle(vector<point> P) {
  mt19937 rnd(1557);
  shuffle(P.begin(), P.end(), rnd);
  point p = P[0];
  double r = 0;
  for (int i = 1; i < P.size(); i++) if (dst(p, P[i]) > r) {
    p = P[i]; r = 0;
    for (int j = 0; j < i; j++) if (dst(p, P[j]) > r) {
      p = 0.5 * (P[i] + P[j]);
      r = dst(p, P[j]);
      for (int k = 0; k < j; k++) if (dst(p, P[k]) > r) {
        p = center(P[i], P[j], P[k]);
        r = dst(p, P[k]);
      }
    }
  }
  return {p, r};
}