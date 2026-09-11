//座標圧縮 Li Chao Tree。直線・線分追加 O(log N)、一点最小値 O(log N)
//xs は query する座標を昇順かつ重複なしで渡す。値と積は ll に収まること
struct LiChaoTree {
    struct Line {
        ll a = 0, b = INF;
        ll get(ll x) const { return a * x + b; }
    };
    int N, siz;
    vector<ll> xs;
    vector<Line> line;

    LiChaoTree(vector<ll> X) : N((int)X.size()), xs(move(X)) {
        siz = 1;
        assert(is_sorted(xs.begin(), xs.end()));
        assert(unique(xs.begin(), xs.end()) == xs.end());
        while (siz < N) siz <<= 1;
        line.resize(2 * siz);
    }
    void addLine(Line x, int k, int l, int r) {
        int m = (l + r) / 2;
        ll xl = xs[min(l, N - 1)], xm = xs[min(m, N - 1)], xr = xs[min(r - 1, N - 1)];
        if (x.get(xm) < line[k].get(xm)) swap(x, line[k]);
        if (r - l == 1) return;
        if (x.get(xl) < line[k].get(xl)) addLine(x, 2 * k, l, m);
        else if (x.get(xr) < line[k].get(xr)) addLine(x, 2 * k + 1, m, r);
    }
    void addLine(ll a, ll b) {
        assert(N > 0);
        addLine({a, b}, 1, 0, siz);
    }
    void addSegment(Line x, int ql, int qr, int k, int l, int r) {
        if (r <= ql or qr <= l) return;
        if (ql <= l and r <= qr) return addLine(x, k, l, r);
        int m = (l + r) / 2;
        addSegment(x, ql, qr, 2 * k, l, m);
        addSegment(x, ql, qr, 2 * k + 1, m, r);
    }
    void addSegment(int l, int r, ll a, ll b) {
        assert(0 <= l and l <= r and r <= N);
        addSegment({a, b}, l, r, 1, 0, siz);
    }
    ll getMin(int p) const {
        assert(0 <= p and p < N);
        ll ret = INF;
        for (int k = p + siz; k; k >>= 1) ret = min(ret, line[k].get(xs[p]));
        return ret;
    }
};
