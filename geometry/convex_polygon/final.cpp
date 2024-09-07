// include 4 basic operators, ccw, point_on_line from int template

// call init() first, then call add_point()
// P should be a convex polygon, counterclokwise, no colinear, at least three points

// point_in_polygon(p) tests if p is strictly contained in the interior of P
// tangent(p, dir) returns -1 if p is strictly inside
// otherwise, returns the vertex number v
// P[v] is the nearest point encountered on the tangent (half)line

// dir = 0 (clockwise), dir = 1 (counterclockwise)
// 다각형 반대방향으로 반직선을 긋고 그 방향으로 돌릴 때 처음으로 만나는 점
struct convex_polygon {
  int n;
  vector<point> P;
  void init() { n = 0; P.clear(); }
  void add_point(point p) { ++n; P.push_back(p); }
  bool sgn(point p) {
    int k = ccw(P[n - 1], P[0], p);
    return k == 0 ? (P[n - 1] - P[0]) * (p - P[0]) >= 0 : k > 0;
  }
  int get_region(point p) {
    if (p.x == P[0].x && p.y == P[0].y) return 0;
    bool k = sgn(p);
    int ss = 1, ee = n - 1;
    while (ss < ee) {
      int mid = (ss + ee) / 2;
      point u = k ? P[0] : P[mid];
      point v = k ? P[mid] : P[0];
      if (ccw(u, v, p) <= 0) ee = mid;
      else ss = mid + 1;
    }
    return k ? ss : -ss;
  }
  bool point_in_polygon(point p) {
    int k = get_region(p);
    if (k <= 0) return false;
    if (ccw(P[0], P[k], p) == 0) {
      if (k == 1 || k == n - 1) return false;
      else return (P[0] - p) * (P[0] - p) < (P[0] - P[k]) * (P[0] - P[k]);
    }
    else return ccw(P[k - 1], P[k], p) > 0;
  }
  int tangent(point p, bool dir) {
    if (point_in_polygon(p)) {
      return -1;
    }
    int k = get_region(p);
    if (k == 0) return dir ? 1 : n - 1;
    if (k > 0) {
      if (p.x == P[k].x && p.y == P[k].y) return dir ? (k + 1) % n : k - 1;
      if (point_on_line(P[k], P[k - 1], p)) return dir ? k : k - 1;
      if (point_on_line(P[0], P[n - 1], p)) return dir ? 0 : n - 1;
    }
    int ss = dir ? (k > 0 ? k : 0) : (k > 0 ? 0 : -k);
    int ee = dir ? (k > 0 ? n : -k) : (k > 0 ? k : n);
    --ee;
    while (ss < ee) {
      int mid = (ss + ee) / 2;
      int k = ccw(p, P[mid], P[mid + 1]);
      if (dir) {
        if (k >= 0) ee = mid;
        else ss = mid + 1;
      }
      else {
        if (k > 0) ss = mid + 1;
        else ee = mid;
      }
    }
    return ss;
  }
};