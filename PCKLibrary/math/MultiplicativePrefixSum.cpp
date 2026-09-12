// Min_25 型の一般の乗法的関数 prefix sum。典型的な計算量 O(N^(3/4) / log N)、空間 O(sqrt N)
// f(1)=1、sum_{p<=n} f(p) と、全素数冪 f(p^e) を callback で与える。
// 使いどころ: N まで配列を持てない範囲で、phi、mu、約数個数、約数和などの総和を求める場合。
// 具体例: f=phi なら N=5 で答えは 1+1+2+2+4=10。
// 使い方:
// PrimeSummatory table(N); // 先に math/PrimeSummatory.cpp を写す
// auto primeValuePrefix=[&](ll n) { return table.primeSum(n)-table.primeCount(n); };
// auto primePower=[](ll p, int e) { ll x=1; while (e--) x*=p; return x-x/p; };
// ll answer=multiplicativePrefixSum<ll>(N, primeValuePrefix, primePower); とすると sum_{i=1}^N phi(i)。
// primeValuePrefix は N/i として現れる引数に O(1) で答えられること。
// 両 callback の f(p) は一致させる。T は加減乗算と T(0), T(1) を持ち、途中の値も範囲内とする。
// sqrt(N) が int の最大値未満であること。
// 乗法的関数 f について sum_{i=1}^N f(i) を返す。N=0 なら空和 0 を返す。
template<class T, class PrimeValuePrefix, class PrimePower>
T multiplicativePrefixSum(ll N, const PrimeValuePrefix& primeValuePrefix,
                          const PrimePower& primePower) {
    assert(N >= 0);
    if (N == 0) return T(0);

    ll root = (ll)sqrtl((long double)N);
    while ((root + 1) <= N / (root + 1)) root++;
    while (root > N / root) root--;
    assert(root < numeric_limits<int>::max());
    int limit = (int)root;

    vector<int> primes;
    vector<bool> composite(limit + 1);
    for (int p = 2; p <= limit; p++) if (!composite[p]) {
        primes.push_back(p);
        if ((ll)p * p <= limit) {
            for (int q = p * p; q <= limit; q += p) composite[q] = true;
        }
    }

    vector<ll> value;
    unordered_map<ll, int> id;
    for (ll l = 1; l <= N;) {
        ll x = N / l;
        id[x] = (int)value.size();
        value.push_back(x);
        ll r = N / x;
        if (r == N) break;
        l = r + 1;
    }

    vector<T> primePrefix(primes.size() + 1, T(0));
    for (int k = 0; k < (int)primes.size(); k++) {
        primePrefix[k + 1] = primePrefix[k] + primePower(primes[k], 1);
    }

    vector<T> sum(value.size());
    for (int i = 0; i < (int)value.size(); i++) {
        sum[i] = primeValuePrefix(value[i]);
    }

    // p より小さい素数を因数に持たない整数の和へ、p を因数に持つ数を戻す。
    for (int k = (int)primes.size() - 1; k >= 0; k--) {
        ll p = primes[k];
        for (int i = 0; i < (int)value.size() and p <= value[i] / p; i++) {
            ll x = value[i], power = p;
            // p^e*m (m>1) と p^(e+1) を加え、初期値に含まれる p は重複させない。
            for (int e = 1; power <= x / p; e++) {
                sum[i] += primePower(p, e)
                    * (sum[id.at(x / power)] - primePrefix[k + 1]);
                power *= p;
                sum[i] += primePower(p, e + 1);
            }
        }
    }

    return T(1) + sum[id.at(N)];
}
