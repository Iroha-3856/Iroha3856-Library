//区分線形凸関数の Slope Trick。各追加 O(log N)、平行移動 O(1)
//getMinInterval() は最小点区間、minValue は最小値
template<class T>
struct SlopeTrick {
    priority_queue<T> left;
    priority_queue<T, vector<T>, greater<T>> right;
    T addLeft = 0, addRight = 0, minValue = 0;

    T topLeft() const { return left.top() + addLeft; }
    T topRight() const { return right.top() + addRight; }
    void pushLeft(T x) { left.push(x - addLeft); }
    void pushRight(T x) { right.push(x - addRight); }
    void addConstant(T x) { minValue += x; }
    void addMaxAminusX(T a) {
        if (!right.empty() and topRight() < a) {
            T x = topRight(); right.pop();
            minValue += a - x;
            pushRight(a); pushLeft(x);
        } else pushLeft(a);
    }
    void addMaxXminusA(T a) {
        if (!left.empty() and a < topLeft()) {
            T x = topLeft(); left.pop();
            minValue += x - a;
            pushLeft(a); pushRight(x);
        } else pushRight(a);
    }
    void addAbs(T a) {
        addMaxAminusX(a);
        addMaxXminusA(a);
    }
    //f(x) <- min_{x-b <= y <= x-a} f(y)
    void shift(T a, T b) {
        assert(a <= b);
        addLeft += a;
        addRight += b;
    }
    void clearLeft() { left = priority_queue<T>(); }
    void clearRight() { right = priority_queue<T, vector<T>, greater<T>>(); }
    pair<optional<T>, optional<T>> getMinInterval() const {
        optional<T> l = left.empty() ? nullopt : optional<T>(topLeft());
        optional<T> r = right.empty() ? nullopt : optional<T>(topRight());
        return {l, r};
    }
};
