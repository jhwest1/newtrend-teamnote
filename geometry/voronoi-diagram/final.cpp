const double eps = 1e-9;
/* include 5 basic operators, intersect from double template */
point rot(point p) { return {-p.y, p.x}; }
namespace voronoi {
  int dcmp(double x) { return x < -eps ? -1 : x > eps ? 1 : 0; }
  double pb_int(point l, point r, double sweepline) {
    if (dcmp(l.y - r.y) == 0) return (l.x + r.x) / 2.0;
    int sign = l.y < r.y ? -1 : 1;
    point v = intersect(l, r - l, point{0, sweepline}, point{1, 0});
    point m = 0.5 * (l + r);
    double d1 = (m - v) * (m - v);
    double d2 = (m - l) * (m - l);
    return v.x + sign * sqrt(max(0.0, d1 - d2));
  }
  point circumcenter(point p, point q, point r) { return intersect(0.5 * (p + q), rot(p - q), 0.5 * (q + r), rot(q - r));	}
  struct Beachline {
    struct node {
      node() {}
      node(point p, int idx) : p(p), idx(idx), end(0), link{0, 0}, par(0), prv(0), nxt(0) {}
      point p; int idx, end;
      node *link[2], *par, *prv, *nxt;
    };
    node *root;
    double sweepline;
    Beachline() : sweepline(-1e20), root(NULL) {}
    inline int dir(node *x) { return x->par->link[0] != x; }
    void rotate(node *n) {
      node *p = n->par; int d = dir(n);
      p->link[d] = n->link[!d];
      if (n->link[!d]) n->link[!d]->par = p;
      n->par = p->par; if (p->par) p->par->link[dir(p)] = n;
      n->link[!d] = p; p->par = n;
    }
    void splay(node *x, node *f = NULL) {
      while (x->par != f) {
        if (x->par->par != f) {
          if (dir(x) == dir(x->par)) rotate(x->par);
          else rotate(x);
        }
        rotate(x);
      }
      if (f == NULL) root = x;
    }
    void insert(node *n, node *p, int d) {
      splay(p); node *c = p->link[d];
      n->link[d] = c; if(c) c->par = n;
      p->link[d] = n; n->par = p;
      node *prv = !d ? p->prv : p;
      node *nxt = !d ? p : p->nxt;
      n->prv = prv; if (prv) prv->nxt = n;
      n->nxt = nxt; if (nxt) nxt->prv = n;
    }
    void erase(node *n) {
      node *prv = n->prv, *nxt = n->nxt;
      if (!prv && !nxt) { if (n == root) root = NULL; return; }
      n->prv = NULL; if (prv) prv->nxt = nxt;
      n->nxt = NULL; if (nxt) nxt->prv = prv;
      splay(n);
      if (!nxt) {
        root->par = NULL; n->link[0] = NULL;
        root = prv;
      }
      else {
        splay(nxt, n); node* c = n->link[0];
        nxt->link[0] = c; c->par = nxt; n->link[0] = NULL;
        n->link[1] = NULL; nxt->par = NULL;
        root = nxt;
      }
    }
    bool get_event(node *cur, double &next_sweep) {
      if (!cur->prv || !cur->nxt) return false;
      point u = rot(cur->p - cur->prv->p);
      point v = rot(cur->nxt->p - cur->p);
      if (dcmp(u/v) != 1) return false;
      point p = circumcenter(cur->p, cur->prv->p, cur->nxt->p);
      double d = sqrt((p - cur->p) * (p - cur->p));
      next_sweep = p.y + d;
      return true;
    }
    node *lower_bound(double x) {
      node *cur = root;
      while (cur) {
        double l = cur->prv ? pb_int(cur->prv->p, cur->p, sweepline) : -1e30;
        double r = cur->nxt ? pb_int(cur->p, cur->nxt->p, sweepline) : 1e30;
        if (l - eps < x && x < r + eps) {
          splay(cur); return cur;
        }
        cur = cur->link[x > r];
      }
    }
  };
  using Bnode = Beachline::node; 
  vector<Bnode*> arr;
  Bnode* new_node(point p, int idx) {
    arr.push_back(new Bnode(p, idx));
    return arr.back();
  }
  struct event {
    event(double sweep, int idx) : type(0), sweep(sweep), idx(idx) {}
    event(double sweep, Bnode *cur): type(1), sweep(sweep), prv(cur->prv->idx), cur(cur), nxt(cur->nxt->idx) {}
    int type, idx, prv, nxt; 
    Bnode* cur;
    double sweep;
    bool operator>(const event &l) const {
      return sweep > l.sweep;
    }
  };
  void VoronoiDiagram(vector<point> &P, vector<point> &V, vector<pii> &E, vector<pii> &D) {
    Beachline bl = Beachline();
    priority_queue<event, vector<event>, greater<event>> events;
    auto add_edge = [&](int u, int v, int a, int b, Bnode* c1, Bnode *c2) {
      if (c1) c1->end = E.size() * 2;
      if (c2) c2->end = E.size() * 2 + 1;
      E.push_back({u, v});
      D.push_back({a, b});
    };
    auto write_edge = [&](int idx, int v) { (idx % 2 == 0 ? E[idx / 2].x : E[idx / 2].y) = v; };
    auto add_event = [&](Bnode *cur) {
      double nxt;
      if (bl.get_event(cur, nxt)) events.push({nxt, cur});
    };
    int n = P.size();
    sort(P.begin(), P.end(), [&](point p, point q) { return p.y == q.y ? p.x < q.x : p.y < q.y; });
    Bnode *prv = new_node(P[0], 0);
    bl.root = prv;
    for (int i = 1; i < n; i++) {
      if (dcmp(P[i].y - P[0].y) == 0) {
        add_edge(-1, -1, i - 1, i, 0, prv);
        Bnode *cur = new_node(P[i], i);
        bl.insert(cur, prv, 1);
        prv = cur;
      }
      else events.push({P[i].y, i});
    }
    while (!events.empty()) {
      event q = events.top(); events.pop();
      Bnode *prv, *cur, *nxt, *site;
      int v = V.size(), idx = q.idx;
      bl.sweepline = q.sweep;
      if (q.type == 0) {
        point p = P[idx];
        cur = bl.lower_bound(p.x);
        bl.insert(site = new_node(p, idx), cur, 0);
        bl.insert(prv = new_node(cur->p, cur->idx), site, 0);
        add_edge(-1, -1, cur->idx, idx, site, prv);
        add_event(prv);
        add_event(cur);
      }
      else {
        cur = q.cur; prv = cur->prv; nxt = cur->nxt;
        if (!prv || !nxt || prv->idx != q.prv || nxt->idx != q.nxt) continue;
        V.push_back(circumcenter(prv->p, nxt->p, cur->p));
        write_edge(prv->end, v);
        write_edge(cur->end, v);
        add_edge(v, -1, prv->idx, nxt->idx, 0, prv);
        bl.erase(cur);
        add_event(prv);
        add_event(nxt);
      }
    }
    arr.clear();
  }
}