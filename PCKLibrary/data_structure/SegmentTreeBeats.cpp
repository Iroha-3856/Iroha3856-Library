// range chmin / chmax / add / sum。0-indexed、半開区間、各操作償却 O(log N)
// 一般化すると、更新ごとに「変化なしの枝刈り条件」と「節点へ一括適用できる条件」を定め、
// 後者を満たさない節点だけ push して降りる。各要素が失敗側へ降りる回数の償却解析が必要
// 使い方:
// SegmentTreeBeats seg(A); とし、rangeChmin(l, r, x), rangeChmax(l, r, x),
// rangeAdd(l, r, x) で半開区間を更新し、rangeSum(l, r) で区間和を得る。
// chmin/chmax は代入ではない。chmin は x より大きい要素だけを x へ下げる。
// 使いどころ: 区間の全要素を上限・下限で切り詰めながら、加算と総和も扱う場合。
// 具体例: {1, 5, 3} に rangeChmin(0, 3, 4) を行うと {1, 4, 3} になり、rangeSum(0, 3)=8。
// 償却計算量はこの四操作の組に対して成立する。任意の更新関数を追加してよいわけではない。
struct SegmentTreeBeats {
    struct Node {
        ll sum = 0, max1 = -INF, max2 = -INF, min1 = INF, min2 = INF, add = 0;
        int maxCount = 0, minCount = 0, len = 0;
    };
    int N, siz;
    vector<Node> node;

    // 初期配列 A から Beats 用の最大・最小・個数・和を構築する。
    SegmentTreeBeats(const vector<ll>& A) : N((int)A.size()) {
        siz = 1;
        while (siz < N) siz <<= 1;
        node.resize(2 * siz);
        for (int i = 0; i < siz; i++) {
            int k = siz + i;
            node[k].len = 1;
            if (i < N) {
                node[k].sum = node[k].max1 = node[k].min1 = A[i];
                node[k].maxCount = node[k].minCount = 1;
            }
        }
        for (int k = siz - 1; k; k--) pull(k);
    }
    // 左右の節点情報を、最大二値・最小二値と個数を保って併合する。
    Node mergeNode(const Node& a, const Node& b) const {
        if (a.maxCount == 0) return b;
        if (b.maxCount == 0) return a;
        Node c;
        c.sum = a.sum + b.sum;
        c.len = a.len + b.len;
        c.max1 = max(a.max1, b.max1);
        c.maxCount = (a.max1 == c.max1 ? a.maxCount : 0)
                   + (b.max1 == c.max1 ? b.maxCount : 0);
        c.max2 = max(a.max1 == c.max1 ? a.max2 : a.max1,
                     b.max1 == c.max1 ? b.max2 : b.max1);
        c.min1 = min(a.min1, b.min1);
        c.minCount = (a.min1 == c.min1 ? a.minCount : 0)
                   + (b.min1 == c.min1 ? b.minCount : 0);
        c.min2 = min(a.min1 == c.min1 ? a.min2 : a.min1,
                     b.min1 == c.min1 ? b.min2 : b.min1);
        return c;
    }
    // 二つの子から節点 k を再計算する。
    void pull(int k) { node[k] = mergeNode(node[2 * k], node[2 * k + 1]); }
    // 節点 k の全実要素へ x を加え、保持情報と遅延加算を更新する。
    void allAdd(int k, ll x) {
        Node& a = node[k];
        if (a.maxCount == 0) return;
        a.sum += x * a.len;
        a.max1 += x;
        if (a.max2 != -INF) a.max2 += x;
        a.min1 += x;
        if (a.min2 != INF) a.min2 += x;
        a.add += x;
    }
    // max2<x<max1 を仮定し、節点 k の最大値だけを x へ下げる。
    void allChmin(int k, ll x) {
        Node& a = node[k];
        if (a.max1 <= x) return;
        a.sum += (x - a.max1) * a.maxCount;
        if (a.min1 == a.max1) a.min1 = x;
        else if (a.min2 == a.max1) a.min2 = x;
        a.max1 = x;
    }
    // min1<x<min2 を仮定し、節点 k の最小値だけを x へ上げる。
    void allChmax(int k, ll x) {
        Node& a = node[k];
        if (x <= a.min1) return;
        a.sum += (x - a.min1) * a.minCount;
        if (a.max1 == a.min1) a.max1 = x;
        else if (a.max2 == a.min1) a.max2 = x;
        a.min1 = x;
    }
    // 節点 k の加算・上限制約・下限制約を二つの子へ伝播する。
    void push(int k) {
        allAdd(2 * k, node[k].add);
        allAdd(2 * k + 1, node[k].add);
        node[k].add = 0;
        allChmin(2 * k, node[k].max1);
        allChmin(2 * k + 1, node[k].max1);
        allChmax(2 * k, node[k].min1);
        allChmax(2 * k + 1, node[k].min1);
    }
    // 問合せ [l, r) と担当 [a, b) に対する range chmin の内部再帰。
    void rangeChmin(int l, int r, ll x, int k, int a, int b) {
        if (b <= l or r <= a or node[k].max1 <= x) return;
        if (l <= a and b <= r and node[k].max2 < x) return allChmin(k, x);
        push(k);
        int m = (a + b) / 2;
        rangeChmin(l, r, x, 2 * k, a, m);
        rangeChmin(l, r, x, 2 * k + 1, m, b);
        pull(k);
    }
    // 問合せ [l, r) と担当 [a, b) に対する range chmax の内部再帰。
    void rangeChmax(int l, int r, ll x, int k, int a, int b) {
        if (b <= l or r <= a or x <= node[k].min1) return;
        if (l <= a and b <= r and x < node[k].min2) return allChmax(k, x);
        push(k);
        int m = (a + b) / 2;
        rangeChmax(l, r, x, 2 * k, a, m);
        rangeChmax(l, r, x, 2 * k + 1, m, b);
        pull(k);
    }
    // 問合せ [l, r) と担当 [a, b) に対する range add の内部再帰。
    void rangeAdd(int l, int r, ll x, int k, int a, int b) {
        if (b <= l or r <= a) return;
        if (l <= a and b <= r) return allAdd(k, x);
        push(k);
        int m = (a + b) / 2;
        rangeAdd(l, r, x, 2 * k, a, m);
        rangeAdd(l, r, x, 2 * k + 1, m, b);
        pull(k);
    }
    // 問合せ [l, r) と担当 [a, b) から区間和を得る内部再帰。
    ll rangeSum(int l, int r, int k, int a, int b) {
        if (b <= l or r <= a) return 0;
        if (l <= a and b <= r) return node[k].sum;
        push(k);
        int m = (a + b) / 2;
        return rangeSum(l, r, 2 * k, a, m)
             + rangeSum(l, r, 2 * k + 1, m, b);
    }
    // 半開区間 [l, r) の各値を min(value, x) へ更新する。
    void rangeChmin(int l, int r, ll x) {
        assert(0 <= l and l <= r and r <= N);
        rangeChmin(l, r, x, 1, 0, siz);
    }
    // 半開区間 [l, r) の各値を max(value, x) へ更新する。
    void rangeChmax(int l, int r, ll x) {
        assert(0 <= l and l <= r and r <= N);
        rangeChmax(l, r, x, 1, 0, siz);
    }
    // 半開区間 [l, r) の各値へ x を加える。
    void rangeAdd(int l, int r, ll x) {
        assert(0 <= l and l <= r and r <= N);
        rangeAdd(l, r, x, 1, 0, siz);
    }
    // 半開区間 [l, r) の要素和を返す。
    ll rangeSum(int l, int r) {
        assert(0 <= l and l <= r and r <= N);
        return rangeSum(l, r, 1, 0, siz);
    }
};
