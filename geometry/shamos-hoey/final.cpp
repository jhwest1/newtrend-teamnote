/* include ccw, point_on_line, intersect from int template here */

// call calc() to check if there exists an intersection
// O(n log n)
int sweep;
struct line {
  point s, e;
  bool operator<(const line &ot) const {
    // compare s.y + (e - s).y / (e - s).x * (sweep - s.x)
    return (__int128)(ot.e.x - ot.s.x) * (1ll * (e.y - s.y) * (sweep - s.x) + 1ll * s.y * (e.x - s.x)) <
           (__int128)(e.x - s.x) * (1ll * (ot.e.y - ot.s.y) * (sweep - ot.s.x) + 1ll * ot.s.y * (ot.e.x - ot.s.x));
  }
};
bool operator<(const line &l, const int &y) { return ccw(l.s, l.e, point{sweep, y}) > 0; }
bool operator<(const int &y, const line &l) { return ccw(l.s, l.e, point{sweep, y}) < 0; }
bool isect(const line &l, const line &r) { return line_intersect(l.s, l.e, r.s, r.e); }

struct event {
  int x, type, idx;
  bool operator<(const event &ot) const { return x == ot.x ? type < ot.type : x < ot.x; }
};
vector<line> L;
vector<event> E;
void add_line(point s, point e) {
  if (s.x > e.x || (s.x == e.x && s.y > e.y)) swap(s, e);
  int idx = (int)L.size();
  L.push_back({s, e});
  if (s.x != e.x) {
    E.push_back({s.x, 0, idx});
    E.push_back({e.x, 2, idx});
  }
  else E.push_back({s.x, 1, idx});
}
bool calc() {
  sort(E.begin(), E.end());
  multiset<line, less<>> ST;
  for (auto &e : E) {
    sweep = e.x;
    line l = L[e.idx];
    if (e.type == 0) {
      auto it = ST.insert(l);
      if (it != ST.begin() && isect(*it, *prev(it))) return true;
      if (it != prev(ST.end()) && isect(*it, *next(it))) return true;
    }
	else if (e.type == 1) {
      if (ST.lower_bound(l.s.y) != ST.upper_bound(l.e.y)) return true;
    }
	else {
      auto it = ST.lower_bound(l);
      if (it != ST.begin() && it != prev(ST.end())) {
        if (isect(*prev(it), *next(it))) return true;
      }
      ST.erase(it);
    }
  }
  return false;
}