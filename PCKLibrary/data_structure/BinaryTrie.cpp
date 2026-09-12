// 非負整数の多重集合。insert / erase / XOR min・max / kth は O(B)
// xorAll(x) は全要素へ XOR を作用。kth は現在の値で 0-indexed
// 使い方:
// BinaryTrie<unsigned, 30> trie; とし、insert(x), erase(x), count(x) で個数を管理する。
// xorAll(mask) は既存の全要素を一括で XOR する。kth(k) は k 番目の値を返す。
// minXor(x) / maxXor(x) は、選んだ要素ではなく x との XOR 値を返す。
// 使いどころ: XOR を基準にした最近値・最遠値や、全要素へ同じ XOR を加える問題。
// 具体例: 5 と 9 を追加すると minXor(6)=3。5 XOR 6 が 3 だからである。
// B は使用する bit 数。値が 0<=x<2^30 なら B=30 とし、不要な上位 bit を持たせない。
template<class T = unsigned long long, int B = 63>
struct BinaryTrie {
    struct Node {
        int child[2] = {-1, -1};
        int count = 0;
    };
    vector<Node> node = vector<Node>(1);
    T lazy = 0;

    // 現在格納されている要素数を重複込みで返す。
    int size() const { return node[0].count; }
    // 要素数が 0 かを返す。
    bool empty() const { return size() == 0; }

    // 値 x を 1 個追加する。遅延 XOR 適用後の見かけの値として x を受け取る。
    void insert(T x) {
        x ^= lazy;
        int v = 0;
        node[v].count++;
        for (int i = B - 1; i >= 0; i--) {
            int b = x >> i & 1;
            if (node[v].child[b] == -1) {
                node[v].child[b] = (int)node.size();
                node.push_back(Node());
            }
            v = node[v].child[b];
            node[v].count++;
        }
    }
    // 値 x の現在の格納個数を返す。
    int count(T x) const {
        x ^= lazy;
        int v = 0;
        for (int i = B - 1; i >= 0; i--) {
            int b = x >> i & 1;
            v = node[v].child[b];
            if (v == -1) return 0;
        }
        return node[v].count;
    }
    // 値 x を 1 個削除する。存在しなければ何もせず false を返す。
    bool erase(T x) {
        if (count(x) == 0) return false;
        x ^= lazy;
        int v = 0;
        node[v].count--;
        for (int i = B - 1; i >= 0; i--) {
            v = node[v].child[x >> i & 1];
            node[v].count--;
        }
        return true;
    }
    // 格納済みの全要素へ x を XOR する。Trie 本体は組み替えず O(1)。
    void xorAll(T x) { lazy ^= x; }

    // 現在値の昇順で 0-indexed の k 番目を返す。0 <= k < size() が必要。
    T kth(int k) const {
        assert(0 <= k and k < size());
        int v = 0;
        T ret = 0;
        for (int i = B - 1; i >= 0; i--) {
            int z = lazy >> i & 1;
            int to = node[v].child[z];
            int cnt = to == -1 ? 0 : node[to].count;
            if (k >= cnt) {
                k -= cnt;
                z ^= 1;
                ret |= T(1) << i;
            }
            v = node[v].child[z];
        }
        return ret;
    }
    // 格納要素 y に対する x XOR y の最小値を返す。空集合では呼べない。
    T minXor(T x) const { return x ^ xorExtremum(x, false); }
    // 格納要素 y に対する x XOR y の最大値を返す。空集合では呼べない。
    T maxXor(T x) const { return x ^ xorExtremum(x, true); }

    // x との XOR が最小または最大になる格納要素 y を返す内部探索。
    T xorExtremum(T x, bool maximum) const {
        assert(!empty());
        int v = 0;
        T ret = 0;
        for (int i = B - 1; i >= 0; i--) {
            int xb = x >> i & 1;
            int lb = lazy >> i & 1;
            int want = xb ^ maximum;
            int raw = want ^ lb;
            int to = node[v].child[raw];
            if (to == -1 or node[to].count == 0) raw ^= 1;
            if ((raw ^ lb) != 0) ret |= T(1) << i;
            v = node[v].child[raw];
        }
        return ret;
    }
};
