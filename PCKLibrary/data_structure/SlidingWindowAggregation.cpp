//queue / deque 上のモノイド積。各操作償却 O(1)、非可換演算に対応
//左から右の順に op を取る。空の prod は e() を返す
template<class S, S(*op)(S, S), S(*e)()>
struct SlidingWindowAggregation {
    struct Node {
        S value, prod;
    };
    vector<Node> left, right;

    int size() const { return (int)left.size() + (int)right.size(); }
    bool empty() const { return left.empty() and right.empty(); }

    void pushFrontRaw(S x) {
        S p = left.empty() ? x : op(x, left.back().prod);
        left.push_back({x, p});
    }
    void pushBackRaw(S x) {
        S p = right.empty() ? x : op(right.back().prod, x);
        right.push_back({x, p});
    }
    vector<S> values() const {
        vector<S> A;
        A.reserve(size());
        for (int i = (int)left.size() - 1; i >= 0; i--) A.push_back(left[i].value);
        for (Node x : right) A.push_back(x.value);
        return A;
    }
    void rebuild() {
        vector<S> A = values();
        left.clear(); right.clear();
        int m = ((int)A.size() + 1) / 2;
        for (int i = m - 1; i >= 0; i--) pushFrontRaw(A[i]);
        for (int i = m; i < (int)A.size(); i++) pushBackRaw(A[i]);
    }
    void pushFront(S x) { pushFrontRaw(x); }
    void pushBack(S x) { pushBackRaw(x); }
    void push(S x) { pushBack(x); }
    void popFront() {
        assert(!empty());
        if (left.empty()) rebuild();
        left.pop_back();
    }
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
    void pop() { popFront(); }
    S front() const {
        assert(!empty());
        if (!left.empty()) return left.back().value;
        return right.front().value;
    }
    S back() const {
        assert(!empty());
        if (!right.empty()) return right.back().value;
        return left.front().value;
    }
    S prod() const {
        if (left.empty() and right.empty()) return e();
        if (left.empty()) return right.back().prod;
        if (right.empty()) return left.back().prod;
        return op(left.back().prod, right.back().prod);
    }
};
