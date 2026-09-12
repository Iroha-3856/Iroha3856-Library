// Lucy DP による pi(N) と sum_{p<=N} p。概ね O(N^(3/4) / log N)、空間 O(sqrt N)
// 使いどころ: N が大きく篩を N まで持てないとき、素数個数または素数和を求める場合。
// 具体例: PrimeSummatory(10) では primeCount(10)=4、primeSum(10)=17。
// 使い方:
// PrimeSummatory table(N); の後 table.primeCount(N), table.primeSum(N) で N 以下を集計する。
// primeCount(x)/primeSum(x) の x は N/i として現れる値、または x<=sqrt(N) に限る。
// 一般の乗法的関数の総和には math/MultiplicativePrefixSum.cpp を使う。
// primeSum は答えが ll に収まる範囲で使い、同じ N に対する複数 query では表を再利用する。
struct PrimeSummatory {
    ll N;
    vector<ll> value, count, sum;
    unordered_map<ll, int> id;

    // 指定 N の商集合について、素数個数と素数和の Lucy DP を構築する。
    PrimeSummatory(ll n) : N(n) {
        for (ll l = 1; l <= N;) {
            ll x = N / l;
            id[x] = (int)value.size();
            value.push_back(x);
            count.push_back(x - 1);
            sum.push_back((ll)((__int128_t)x * (x + 1) / 2 - 1));
            l = N / x + 1;
        }
        for (ll p = 2; p <= N / p; p++) {
            if (primeCount(p) == primeCount(p - 1)) continue;
            ll beforeCount = primeCount(p - 1), beforeSum = primeSum(p - 1);
            for (int i = 0; i < (int)value.size() and value[i] >= p * p; i++) {
                ll x = value[i];
                count[i] -= primeCount(x / p) - beforeCount;
                sum[i] -= p * (primeSum(x / p) - beforeSum);
            }
        }
    }
    // x 以下の素数個数を返す。x は構築時の商集合に含まれる必要がある。
    ll primeCount(ll x) const {
        if (x < 2) return 0;
        return count[id.at(x)];
    }
    // x 以下の素数の総和を返す。x は構築時の商集合に含まれる必要がある。
    ll primeSum(ll x) const {
        if (x < 2) return 0;
        return sum[id.at(x)];
    }
};
