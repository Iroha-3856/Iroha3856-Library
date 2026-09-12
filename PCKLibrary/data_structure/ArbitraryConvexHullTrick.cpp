ll div_floor(ll a, ll b) { return a / b - (a % b < 0); }
struct Line {
    mutable ll k, m, p;
    bool operator<(Line o) const { return k < o.k; }
    bool operator<(ll x) const { return p < x; }
};
struct LineContainer : set<Line, less<>> {
    bool isect(iterator x, iterator y) {
        x->p = div_floor(x->m - y->m, y->k - x->k);
        return x->p >= y->p;
    }
    void add(ll k, ll m) {
        auto [x, f] = insert(Line{k, m, LLONG_MAX});
        if (!f) chmax(x->m, m);
        auto y = next(x);
        while (y != end() && isect(x, y)) y = erase(y);
        if (x != begin() && isect(--x, --y)) isect(x, y = erase(y));
        while (x != begin() && (--x)->p >= (--y)->p) isect(x, y = erase(y));
    }
    ll max(ll x) {
        auto [k, m, p] = *lower_bound(x);
        return k * x + m;
    }
};
