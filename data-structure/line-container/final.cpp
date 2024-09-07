const ll INF = 9e18;
struct Line {
  mutable ll a, b, p;
  bool operator<(const Line& o) const { return a != o.a ? o.a < a : b < o.b; }  // min : < , max : > (both)
  bool operator<(ll x) const { return p < x; }
};
// LineContainer::LineContainer cht;
// multiset of Line, sorted in increasing order of cross points
struct LineContainer : multiset<Line, less<>> {
  // for doubles, use INF = 1/.0, div(a,b) = a/b
  ll div(ll u, ll d) { return u / d - ((u ^ d) < 0 && u % d); }
  bool isect(iterator x, iterator y) {
    if (y == end()) {
      x->p = INF;
      return 0;
    }
    if (x->a == y->a) x->p = x->b < y->b ? INF : -INF;  // min : < , max : >
    else x->p = div(y->b - x->b, x->a - y->a);
    return x->p >= y->p;
  }
  void push(ll a, ll b) {
    auto z = insert({a, b, 0}), y = z++, x = y;
    while (isect(y, z)) z = erase(z);
    if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
    while ((y = x) != begin() && (--x)->p >= y->p) isect(x, erase(y));
  }
  ll query(ll x) {
    assert(!empty());
    auto l = *lower_bound(x);
    return l.a * x + l.b;
  }
};