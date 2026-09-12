// 区分線形凸関数の Slope Trick。各追加 O(log N)、平行移動 O(1)
// getMinInterval() は最小点区間、minValue は最小値
// 使いどころ: 絶対値・片側罰金・時刻間の移動制約からなる一次元凸 DP を関数ごと保持する場合。
// 具体例: addAbs(2), addAbs(5) の後、minValue=3、最小点区間は [2, 5]。
// 使い方:
// SlopeTrick<ll> f; は f(x)=0 から始まる。addAbs(a) で |x-a| を加える。
// addMaxAminusX(a) は max(a-x, 0)、addMaxXminusA(a) は max(x-a, 0) を加える。
// shift(a, b) は f(x)<-min_{x-b<=y<=x-a}f(y)。累積最小化は clearLeft/clearRight を使う。
// f.minValue が最小値、getMinInterval() が最小点区間の端。無限端は nullopt。
template<class T>
struct SlopeTrick {
    priority_queue<T> left;
    priority_queue<T, vector<T>, greater<T>> right;
    T addLeft = 0, addRight = 0, minValue = 0;

    // 左 heap の最大 breakpoint に遅延平行移動を反映して返す。
    T topLeft() const { return left.top() + addLeft; }
    // 右 heap の最小 breakpoint に遅延平行移動を反映して返す。
    T topRight() const { return right.top() + addRight; }
    // 実座標 x を左 heap へ遅延量を除いて追加する。
    void pushLeft(T x) { left.push(x - addLeft); }
    // 実座標 x を右 heap へ遅延量を除いて追加する。
    void pushRight(T x) { right.push(x - addRight); }
    // 関数全体へ定数 x を加える。
    void addConstant(T x) { minValue += x; }
    // 関数へ max(a-x, 0) を加える。
    void addMaxAminusX(T a) {
        if (!right.empty() and topRight() < a) {
            T x = topRight(); right.pop();
            minValue += a - x;
            pushRight(a); pushLeft(x);
        } else pushLeft(a);
    }
    // 関数へ max(x-a, 0) を加える。
    void addMaxXminusA(T a) {
        if (!left.empty() and a < topLeft()) {
            T x = topLeft(); left.pop();
            minValue += x - a;
            pushLeft(a); pushRight(x);
        } else pushRight(a);
    }
    // 関数へ |x-a| を加える。
    void addAbs(T a) {
        addMaxAminusX(a);
        addMaxXminusA(a);
    }
    // f(x) を min_{x-b<=y<=x-a} f(y) へ更新する。a<=b が必要。
    void shift(T a, T b) {
        assert(a <= b);
        addLeft += a;
        addRight += b;
    }
    // f(x) を min_{y<=x} f(y) へ更新し、左側の傾きを消す。
    void clearLeft() { left = priority_queue<T>(); }
    // f(x) を min_{x<=y} f(y) へ更新し、右側の傾きを消す。
    void clearRight() { right = priority_queue<T, vector<T>, greater<T>>(); }
    // 最小値を取る閉区間の {左端, 右端} を返す。無限端は nullopt。
    pair<optional<T>, optional<T>> getMinInterval() const {
        optional<T> l = left.empty() ? nullopt : optional<T>(topLeft());
        optional<T> r = right.empty() ? nullopt : optional<T>(topRight());
        return {l, r};
    }
};
