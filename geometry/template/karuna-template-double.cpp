const double eps = 1e-9;
struct point {
  double x, y;
};
point operator+(point a, point b) { return {a.x + b.x, a.y + b.y}; }
point operator-(point a, point b) { return {a.x - b.x, a.y - b.y}; }
double operator*(point a, point b) { return a.x * b.x + a.y * b.y; }
double operator/(point a, point b) { return a.x * b.y - a.y * b.x; }
point operator*(double k, point a) { return {k * a.x, k * a.y}; }

point unit(point a) { return 1 / sqrt(a * a) * a; }

int ccw(point p, point q, point r) {
  double x = (q - p) / (r - p);
  return (x > eps) - (x < -eps);
}
bool point_on_line(point p, point q, point r) {
  double x = (r - p) * (q - p);
  return ccw(p, q, r) == 0 && -eps < x && x < (q - p) * (q - p) + eps;
}
point intersect(point p, point u, point q, point v) {
  return p + (((q - p) / v) / (u / v)) * u;
}
// 교점이 없으면 0, 교점이 선분의 끝점이면 1, 두 선분이 교차하면 2, 무수히 많으면 3
int line_intersect_verbose(point p, point q, point r, point s, point &a, point &b) {
  const double eps = 1e-6;
  if (ccw(p, q, r) == 0 && ccw(p, q, s) == 0) {
    double u = (r - p) * (q - p);
    double v = (s - p) * (q - p);
    double l = (q - p) * (q - p);
    if (u > v) swap(u, v);
    // intersection of [0, l] and [u, v]
    // (u의 scale) * (자료형의 상대오차 정밀도)가 eps보다 작도록.
    // (좌표의 차이) * (선분의 길이) 가 eps보다 크도록.
    if (l < u - eps || 0 > v + eps) return 0;
    else if (abs(u - l) < eps) { a = p; return 1; } 
	  else if (abs(0 - v) < eps) { a = q; return 1; } 
	  else {
      a = p + (max(0.0, u) / l) * (q - p);
      b = p + (min(l, v) / l) * (q - p);
      return 3;
    }
  }
  if (point_on_line(r, s, p)) { a = p; return 1; }
  if (point_on_line(r, s, q)) { a = q; return 1; }
  if (point_on_line(p, q, r)) { a = r; return 1; }
  if (point_on_line(p, q, s)) { a = s; return 1; }
  if (ccw(p, q, r) * ccw(p, q, s) < 0 && ccw(r, s, p) * ccw(r, s, q) < 0) {
    a = intersect(p, q - p, r, s - r);
    return 2;
  }
  return 0;
}