// オフライン二次元 Fenwick Tree。点加算と半開長方形和を O(log^2 N) で処理する。
// 構築 O(N log N)、空間 O(N log N)。更新する座標 (x, y) は構築前に全て列挙する。
// 使いどころ: 座標が大きい・疎な二次元点集合に、点加算と長方形和が交互に現れる場合。
// 使い方:
// OfflineFenwickTree2D<ll> bit({{2, 3}, {5, 7}});
// bit.add(2, 3, 4); bit.add(5, 7, 6);
// bit.sum(2, 6, 3, 8) == 10; // x は [2,6)、y は [3,8)
// 登録点の重複は許す。登録していない点への add は不可。sum の端点は未登録でもよい。
// T は T{} が加法単位元で、+= / -= が使える可換な加法群を仮定する。
// 座標は ll、積算値は T に収まること。LLONG_MAX の点は登録・更新できるが、
// 半開区間の上端をそれより大きく指定できないため、その点を範囲和には含められない。
template<class T>
struct OfflineFenwickTree2D {
private:
    vector<pair<ll, ll>> points;
    vector<ll> xs;
    vector<vector<ll>> ys;
    vector<vector<T>> bit;

public:
    // 点加算の候補座標 P を登録し、内部の二重 Fenwick Tree を構築する。
    OfflineFenwickTree2D(vector<pair<ll, ll>> P) {
        sort(P.begin(), P.end());
        P.erase(unique(P.begin(), P.end()), P.end());
        points = P;
        for (auto [x, y] : P) xs.push_back(x);
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        int N = (int)xs.size();
        ys.resize(N + 1);
        bit.resize(N + 1);
        // y 順に配ると、各 x Fenwick 節点の ys もソート済みになる。
        sort(P.begin(), P.end(), [](auto a, auto b) {
            return a.second != b.second ? a.second < b.second : a.first < b.first;
        });
        for (auto [x, y] : P) {
            int i = (int)(lower_bound(xs.begin(), xs.end(), x) - xs.begin()) + 1;
            for (; i <= N; i += i & -i) ys[i].push_back(y);
        }
        for (int i = 1; i <= N; i++) {
            ys[i].erase(unique(ys[i].begin(), ys[i].end()), ys[i].end());
            bit[i].assign(ys[i].size() + 1, T{});
        }
    }

    // 登録済みの点 (x, y) の重みへ delta を加える。差し引くなら -delta を渡す。
    void add(ll x, ll y, T delta) {
        assert(binary_search(points.begin(), points.end(), pair<ll, ll>{x, y}));
        int N = (int)xs.size();
        int i = (int)(lower_bound(xs.begin(), xs.end(), x) - xs.begin()) + 1;
        for (; i <= N; i += i & -i) {
            int j = (int)(lower_bound(ys[i].begin(), ys[i].end(), y) - ys[i].begin()) + 1;
            for (; j < (int)bit[i].size(); j += j & -j) bit[i][j] += delta;
        }
    }

    // 座標 x' < x, y' < y を満たす点の重みの合計を返す。端点は登録不要。
    T prefixSum(ll x, ll y) const {
        T ret{};
        int i = (int)(lower_bound(xs.begin(), xs.end(), x) - xs.begin());
        for (; i > 0; i -= i & -i) {
            int j = (int)(lower_bound(ys[i].begin(), ys[i].end(), y) - ys[i].begin());
            for (; j > 0; j -= j & -j) ret += bit[i][j];
        }
        return ret;
    }

    // x in [lx, rx), y in [ly, ry) の長方形和を返す。空長方形なら T{}。
    T sum(ll lx, ll rx, ll ly, ll ry) const {
        assert(lx <= rx and ly <= ry);
        if (lx == rx or ly == ry) return T{};
        T ret = prefixSum(rx, ry);
        ret -= prefixSum(lx, ry);
        ret -= prefixSum(rx, ly);
        ret += prefixSum(lx, ly);
        return ret;
    }
};
