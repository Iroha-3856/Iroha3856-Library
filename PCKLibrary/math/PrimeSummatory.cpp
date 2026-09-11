//Lucy DP による pi(N), sum_{p<=N} p と、商列挙再帰による sum phi, sum mu
//PrimeSummatory は概ね O(N^(3/4) / log N)、Phi/Mu は前計算 L 後 O(sqrt N) 状態
struct PrimeSummatory {
    ll N;
    vector<ll> value, count, sum;
    unordered_map<ll, int> id;

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
    ll primeCount(ll x) const {
        if (x < 2) return 0;
        return count[id.at(x)];
    }
    ll primeSum(ll x) const {
        if (x < 2) return 0;
        return sum[id.at(x)];
    }
};

struct PhiMobiusSummatory {
    int limit;
    vector<ll> phiPrefix, mobiusPrefix;
    unordered_map<ll, ll> phiMemo, mobiusMemo;

    PhiMobiusSummatory(int L) : limit(L), phiPrefix(L + 1), mobiusPrefix(L + 1) {
        vector<int> primes, phi(L + 1), mu(L + 1);
        vector<bool> composite(L + 1);
        if (L >= 1) phi[1] = mu[1] = 1;
        for (int i = 2; i <= L; i++) {
            if (!composite[i]) primes.push_back(i), phi[i] = i - 1, mu[i] = -1;
            for (int p : primes) {
                if ((ll)i * p > L) break;
                composite[i * p] = true;
                if (i % p == 0) {
                    phi[i * p] = phi[i] * p;
                    mu[i * p] = 0;
                    break;
                }
                phi[i * p] = phi[i] * (p - 1);
                mu[i * p] = -mu[i];
            }
        }
        for (int i = 1; i <= L; i++) {
            phiPrefix[i] = phiPrefix[i - 1] + phi[i];
            mobiusPrefix[i] = mobiusPrefix[i - 1] + mu[i];
        }
    }
    ll sumPhi(ll n) {
        if (n <= limit) return phiPrefix[(int)n];
        if (phiMemo.count(n)) return phiMemo[n];
        ll ret = (ll)((__int128_t)n * (n + 1) / 2);
        for (ll l = 2; l <= n;) {
            ll q = n / l, r = n / q + 1;
            ret -= (r - l) * sumPhi(q);
            l = r;
        }
        return phiMemo[n] = ret;
    }
    ll sumMobius(ll n) {
        if (n <= limit) return mobiusPrefix[(int)n];
        if (mobiusMemo.count(n)) return mobiusMemo[n];
        ll ret = 1;
        for (ll l = 2; l <= n;) {
            ll q = n / l, r = n / q + 1;
            ret -= (r - l) * sumMobius(q);
            l = r;
        }
        return mobiusMemo[n] = ret;
    }
};
