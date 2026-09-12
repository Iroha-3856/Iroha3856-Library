// 暗黙 Treap。挿入・削除・一点更新・反転・区間積は期待 O(log N)
// 0-indexed、半開区間 [l, r)。prod は非可換演算に対応
// 使い方:
// S op(S left, S right), S e() を定義して ImplicitTreap<S, op, e> sequence;
// insert(k, x) は k 番目の直前へ挿入、erase(k) は削除値を返し、set(k, x) は置換する。
// reverse(l, r) と prod(l, r) は半開区間。get(k), size() も利用できる。
// 使いどころ: 要素の挿入・削除で添字が動く列に対し、反転や区間集計も行う場合。
// 具体例: a, b, c の順に挿入して reverse(0, 3) すると列は c, b, a になる。
// S は要素と区間積の型。op は列順を保って結合し、e は空列の値を返す。
template<class S, S(*op)(S, S), S(*e)()>
struct ImplicitTreap {
    struct Node {
        S value, prod, revProd;
        unsigned priority;
        int siz;
        bool rev;
        Node *left, *right;

        // 値 x と乱択優先度 p を持つ単一要素の節点を作る。
        Node(S x, unsigned p) : value(x), prod(x), revProd(x), priority(p),
                               rev(false), left(nullptr), right(nullptr) {
            siz = 1;
        }
    };
    Node *root = nullptr;
    unsigned seed = 2463534242U;

    // 部分木 t の要素数を返す。空部分木は 0。
    int size(Node *t) const { return t == nullptr ? 0 : t->siz; }
    // 部分木 t の正順積を返す。空部分木は e()。
    S prod(Node *t) const { return t == nullptr ? e() : t->prod; }
    // 部分木 t の逆順積を返す。空部分木は e()。
    S revProd(Node *t) const { return t == nullptr ? e() : t->revProd; }
    // xorshift により次の節点優先度を生成する。
    unsigned random() {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        return seed;
    }
    // 部分木 t へ反転作用を適用し、子・正順積・逆順積を入れ替える。
    void applyReverse(Node *t) {
        if (t == nullptr) return;
        swap(t->left, t->right);
        swap(t->prod, t->revProd);
        t->rev ^= true;
    }
    // t に保留された反転作用を左右の子へ伝播する。
    void push(Node *t) {
        if (t == nullptr or !t->rev) return;
        applyReverse(t->left);
        applyReverse(t->right);
        t->rev = false;
    }
    // 子の情報から t の要素数と正順・逆順積を再計算する。
    void update(Node *t) {
        if (t == nullptr) return;
        t->siz = 1 + size(t->left) + size(t->right);
        t->prod = op(op(prod(t->left), t->value), prod(t->right));
        t->revProd = op(op(revProd(t->right), t->value), revProd(t->left));
    }
    // 列 t を先頭 k 要素と残りへ分割して {left, right} を返す。
    pair<Node*, Node*> split(Node *t, int k) {
        if (t == nullptr) return {nullptr, nullptr};
        push(t);
        if (k <= size(t->left)) {
            auto [a, b] = split(t->left, k);
            t->left = b;
            update(t);
            return {a, t};
        }
        auto [a, b] = split(t->right, k - size(t->left) - 1);
        t->right = a;
        update(t);
        return {t, b};
    }
    // a の全要素が b より前にある二列を連結して根を返す。
    Node *merge(Node *a, Node *b) {
        if (a == nullptr) return b;
        if (b == nullptr) return a;
        if (a->priority > b->priority) {
            push(a);
            a->right = merge(a->right, b);
            update(a);
            return a;
        }
        push(b);
        b->left = merge(a, b->left);
        update(b);
        return b;
    }
    // 列全体の現在の要素数を返す。
    int size() const { return size(root); }
    // x を 0-indexed の位置 k の直前へ挿入する。末尾挿入では k=size()。
    void insert(int k, S x) {
        assert(0 <= k and k <= size());
        auto [a, b] = split(root, k);
        root = merge(merge(a, new Node(x, random())), b);
    }
    // 位置 k の要素を削除し、その値を返す。節点領域は解放しない。
    S erase(int k) {
        assert(0 <= k and k < size());
        auto [a, c] = split(root, k);
        auto [b, d] = split(c, 1);
        S ret = b->value;
        root = merge(a, d);
        return ret;
    }
    // 位置 k の値を x に置き換える。
    void set(int k, S x) {
        assert(0 <= k and k < size());
        auto [a, c] = split(root, k);
        auto [b, d] = split(c, 1);
        b->value = x;
        update(b);
        root = merge(a, merge(b, d));
    }
    // 位置 k の現在値を返す。
    S get(int k) { return prod(k, k + 1); }
    // 半開区間 [l, r) の要素順を反転する。
    void reverse(int l, int r) {
        assert(0 <= l and l <= r and r <= size());
        auto [a, c] = split(root, l);
        auto [b, d] = split(c, r - l);
        applyReverse(b);
        root = merge(a, merge(b, d));
    }
    // 半開区間 [l, r) を列順に畳み込む。分割後は元の列へ戻す。
    S prod(int l, int r) {
        assert(0 <= l and l <= r and r <= size());
        auto [a, c] = split(root, l);
        auto [b, d] = split(c, r - l);
        S ret = prod(b);
        root = merge(a, merge(b, d));
        return ret;
    }
};
