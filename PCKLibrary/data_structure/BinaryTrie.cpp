//非負整数の多重集合。insert / erase / XOR min・max / kth は O(B)
//xorAll(x) は全要素へ XOR を作用。kth は現在の値で 0-indexed
template<class T = unsigned long long, int B = 63>
struct BinaryTrie {
    struct Node {
        int child[2] = {-1, -1};
        int count = 0;
    };
    vector<Node> node = vector<Node>(1);
    T lazy = 0;

    int size() const { return node[0].count; }
    bool empty() const { return size() == 0; }

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
    void xorAll(T x) { lazy ^= x; }

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
    T minXor(T x) const { return x ^ xorExtremum(x, false); }
    T maxXor(T x) const { return x ^ xorExtremum(x, true); }

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
