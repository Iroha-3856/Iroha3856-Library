// queue / deque 上のモノイド積。各操作償却 O(1)、非可換演算に対応
// 左から右の順に op を取る。空の prod は e() を返す
// 使い方:
// SlidingWindowAggregation<S, op, e> q; に pushFront/pushBack で値を入れる。
// front/back は端の値、prod() は先頭から末尾までの積を返す。
// push(x), pop() は通常の queue と同じく pushBack(x), popFront() の別名。
// 使いどころ: sliding window の左右端を動かしながら、窓全体の積を O(1) 償却で求める場合。
// 具体例: 文字列 a, b, c を順に pushBack し、op=結合なら prod() は "abc"。
// left/right の二 stack は内部実装であり、利用側は通常の deque と同じ向きだけ意識すればよい。
template<class S, S(*op)(S, S), S(*e)()>
struct SlidingWindowAggregation {
    struct Node {
        S value, prod;
    };
    vector<Node> left, right;

    // deque の現在の要素数を返す。
    int size() const { return (int)left.size() + (int)right.size(); }
    // deque が空かを返す。
    bool empty() const { return left.empty() and right.empty(); }

    // 再構築を伴わず left stack へ x を追加する内部操作。
    void pushFrontRaw(S x) {
        S p = left.empty() ? x : op(x, left.back().prod);
        left.push_back({x, p});
    }
    // 再構築を伴わず right stack へ x を追加する内部操作。
    void pushBackRaw(S x) {
        S p = right.empty() ? x : op(right.back().prod, x);
        right.push_back({x, p});
    }
    // 現在の要素を front から back の順に取り出す内部関数。
    vector<S> values() const {
        vector<S> A;
        A.reserve(size());
        for (int i = (int)left.size() - 1; i >= 0; i--) A.push_back(left[i].value);
        for (Node x : right) A.push_back(x.value);
        return A;
    }
    // 全要素を二つの stack へほぼ半分ずつ再配置し、以後の端操作を可能にする。
    void rebuild() {
        vector<S> A = values();
        left.clear(); right.clear();
        int m = ((int)A.size() + 1) / 2;
        for (int i = m - 1; i >= 0; i--) pushFrontRaw(A[i]);
        for (int i = m; i < (int)A.size(); i++) pushBackRaw(A[i]);
    }
    // x を deque の先頭へ追加する。
    void pushFront(S x) { pushFrontRaw(x); }
    // x を deque の末尾へ追加する。
    void pushBack(S x) { pushBackRaw(x); }
    // queue 用の別名。x を末尾へ追加する。
    void push(S x) { pushBack(x); }
    // 先頭要素を削除する。空 deque では呼べない。
    void popFront() {
        assert(!empty());
        if (left.empty()) rebuild();
        left.pop_back();
    }
    // 末尾要素を削除する。空 deque では呼べない。
    void popBack() {
        assert(!empty());
        if (right.empty()) {
            vector<S> A = values();
            left.clear(); right.clear();
            int m = (int)A.size() / 2;
            for (int i = m - 1; i >= 0; i--) pushFrontRaw(A[i]);
            for (int i = m; i < (int)A.size(); i++) pushBackRaw(A[i]);
        }
        right.pop_back();
    }
    // queue 用の別名。先頭要素を削除する。
    void pop() { popFront(); }
    // 先頭要素を返す。空 deque では呼べない。
    S front() const {
        assert(!empty());
        if (!left.empty()) return left.back().value;
        return right.front().value;
    }
    // 末尾要素を返す。空 deque では呼べない。
    S back() const {
        assert(!empty());
        if (!right.empty()) return right.back().value;
        return left.front().value;
    }
    // front から back の順に全要素を畳み込む。空なら e() を返す。
    S prod() const {
        if (left.empty() and right.empty()) return e();
        if (left.empty()) return right.back().prod;
        if (right.empty()) return left.back().prod;
        return op(left.back().prod, right.back().prod);
    }
};
