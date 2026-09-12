// 座標圧縮 Li Chao Tree。直線・線分追加 O(log N)、一点最小値 O(log N)
// xs は query する座標を昇順かつ重複なしで渡す。値と積は ll に収まること
// 使い方:
// LiChaoTree tree(xs); の xs に、後で問い合わせる x 座標をすべて渡す。
// addLine(a, b) で y = ax + b を全域へ、addSegment(l, r, a, b) で添字 [l, r) へ追加する。
// getMin(p) は座標値ではなく xs の添字 p を受け取る。直線がなければ INF を返す。
// 使いどころ: DP の遷移を一次関数の最小値へ変形でき、問い合わせ x を事前に列挙できる場合。
// 具体例: xs={-2, 0, 5} に y=3x+1 を追加すると getMin(2)=16。添字 2 の座標は 5 である。
// addSegment の l, r も x 座標ではなく xs 上の添字。実座標で区間を切るなら lower_bound で変換する。
struct LiChaoTree {
    struct Line {
        ll a = 0, b = INF;
        // 座標 x における直線 ax+b の値を返す。
        ll get(ll x) const { return a * x + b; }
    };
    int N, siz;
    vector<ll> xs;
    vector<Line> line;

    // 問い合わせ座標 X を受け取り、空の最小値 Li Chao Tree を構築する。
    LiChaoTree(vector<ll> X) : N((int)X.size()), xs(move(X)) {
        siz = 1;
        assert(is_sorted(xs.begin(), xs.end()));
        assert(unique(xs.begin(), xs.end()) == xs.end());
        while (siz < N) siz <<= 1;
        line.resize(2 * siz);
    }
    // 節点 k の担当区間 [l, r) へ直線 x を追加する内部再帰。
    void addLine(Line x, int k, int l, int r) {
        int m = (l + r) / 2;
        ll xl = xs[min(l, N - 1)], xm = xs[min(m, N - 1)], xr = xs[min(r - 1, N - 1)];
        if (x.get(xm) < line[k].get(xm)) swap(x, line[k]);
        if (r - l == 1) return;
        if (x.get(xl) < line[k].get(xl)) addLine(x, 2 * k, l, m);
        else if (x.get(xr) < line[k].get(xr)) addLine(x, 2 * k + 1, m, r);
    }
    // 全問い合わせ座標に対して直線 y=ax+b を追加する。
    void addLine(ll a, ll b) {
        assert(N > 0);
        addLine({a, b}, 1, 0, siz);
    }
    // 添字区間 [ql, qr) を覆う節点へ直線 x を追加する内部再帰。
    void addSegment(Line x, int ql, int qr, int k, int l, int r) {
        if (r <= ql or qr <= l) return;
        if (ql <= l and r <= qr) return addLine(x, k, l, r);
        int m = (l + r) / 2;
        addSegment(x, ql, qr, 2 * k, l, m);
        addSegment(x, ql, qr, 2 * k + 1, m, r);
    }
    // 問い合わせ座標の添字区間 [l, r) に限って直線 y=ax+b を追加する。
    void addSegment(int l, int r, ll a, ll b) {
        assert(0 <= l and l <= r and r <= N);
        addSegment({a, b}, l, r, 1, 0, siz);
    }
    // 座標 xs[p] における追加済み直線の最小値を返す。直線がなければ INF。
    ll getMin(int p) const {
        assert(0 <= p and p < N);
        ll ret = INF;
        for (int k = p + siz; k; k >>= 1) ret = min(ret, line[k].get(xs[p]));
        return ret;
    }
};
