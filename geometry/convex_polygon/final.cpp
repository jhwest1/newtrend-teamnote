// include 4 basic operators, ccw, point_on_line from int template

// call init() first, then after adding points, call convex_hull()
// need not call convex_hull() if P is guaranteed to be convex and no 3 collinear points
// point_in_polygon(p) tests if p is strictly contained in the interior of P
// tangent(p, dir) returns -1 if p is strictly inside
// otherwise, returns the vertex number v
// P[v] is the nearest point encountered on the tangent (half)line
struct convex_polygon {
  int n;
  vector<point> P;
  void init() { n = 0; P.clear(); }
  void add_point(point p) { ++n; P.push_back(p); }
  void convex_hull() {
    // MUST remove duplicate points if exists
    int n = P.size();
    for (int i = 1; i < n; i++) {
      if (P[i].x < P[0].x || (P[i].x == P[0].x && P[i].y < P[0].y)) swap(P[0], P[i]);
    }
    sort(P.begin() + 1, P.end(), [&](point p, point q) {
      int k = ccw(P[0], p, q);
      return k == 0 ? (P[0] - p) * (P[0] - p) < (P[0] - q) * (P[0] - q) : k > 0;
    });
    vector<point> C;
    for (int i = 0; i < n; i++) {
      while (C.size() > 1 && ccw(C[C.size() - 2], C.back(), P[i]) <= 0) C.pop_back();
      C.push_back(P[i]);
    }
    P = C;
    n = P.size();
  }

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
  int tangent(point p, bool dir) { // dir = 1 is counterclockwise
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