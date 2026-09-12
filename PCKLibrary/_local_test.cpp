#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "data_structure/Set64.cpp"
#include "math/Modint.cpp"
#include "math/PrimeSummatory.cpp"
#include "math/MultiplicativePrefixSum.cpp"

int main() {
    for (int N : {0, 1, 2, 63, 64, 65, 127, 128, 129}) {
        Set64 actual(N);
        set<int> expected;
        for (int x = 0; x < N; x++) if (x % 3 != 1) {
            assert(actual.insert(x));
            assert(!actual.insert(x));
            expected.insert(x);
        }
        assert(actual.size() == (int)expected.size());
        for (int x = -2; x <= N + 2; x++) {
            auto it = expected.lower_bound(x);
            int lower = it == expected.end() ? N : *it;
            assert(actual.lowerBound(x) == lower);

            it = expected.upper_bound(x);
            int upper = it == expected.end() ? N : *it;
            assert(actual.upperBound(x) == upper);

            it = expected.upper_bound(x);
            int previous = it == expected.begin() ? -1 : *prev(it);
            assert(actual.predecessor(x) == previous);
        }
        for (int x = 0; x < N; x += 2) {
            bool erased = expected.erase(x);
            assert(actual.erase(x) == erased);
        }
        assert(actual.empty() == expected.empty());
    }

    mt19937 random(0);
    for (int N : {1, 2, 63, 64, 65, 4095, 4096, 4097, 100000, 262145}) {
        Set64 actual(N);
        set<int> expected;
        for (int query = 0; query < 20000; query++) {
            int x = (int)(random() % (N + 5)) - 2;
            int type = (int)(random() % 6);
            if (type == 0 and 0 <= x and x < N) {
                assert(actual.insert(x) == expected.insert(x).second);
            } else if (type == 1 and 0 <= x and x < N) {
                assert(actual.erase(x) == (expected.erase(x) != 0));
            } else if (type == 2) {
                assert(actual.contains(x) == (expected.count(x) != 0));
            } else if (type == 3) {
                auto it = expected.lower_bound(x);
                int answer = it == expected.end() ? N : *it;
                assert(actual.lowerBound(x) == answer);
            } else if (type == 4) {
                auto it = expected.upper_bound(x);
                int answer = it == expected.end() ? N : *it;
                assert(actual.upperBound(x) == answer);
            } else {
                auto it = expected.upper_bound(x);
                int answer = it == expected.begin() ? -1 : *prev(it);
                assert(actual.predecessor(x) == answer);
            }
            assert(actual.size() == (int)expected.size());
            assert(actual.empty() == expected.empty());
            assert(actual.minimum() == (expected.empty() ? N : *expected.begin()));
            assert(actual.maximum() == (expected.empty() ? -1 : *expected.rbegin()));
        }
    }

    auto phiPrimePower = [](ll p, int e) {
        ll x = 1;
        while (e--) x *= p;
        return x - x / p;
    };
    auto muPrimePower = [](ll, int e) { return e == 1 ? -1LL : 0LL; };
    auto divisorCountPrimePower = [](ll, int e) { return (ll)e + 1; };
    auto divisorSumPrimePower = [](ll p, int e) {
        ll sum = 1, x = 1;
        while (e--) x *= p, sum += x;
        return sum;
    };

    ll phiSum = 0, muSum = 0, divisorCountSum = 0, divisorSumSum = 0;
    for (int N = 0; N <= 300; N++) {
        PrimeSummatory table(N);
        auto phiPrimePrefix = [&](ll n) { return table.primeSum(n) - table.primeCount(n); };
        auto muPrimePrefix = [&](ll n) { return -table.primeCount(n); };
        auto divisorCountPrimePrefix = [&](ll n) { return 2 * table.primeCount(n); };
        auto divisorSumPrimePrefix = [&](ll n) { return table.primeSum(n) + table.primeCount(n); };
        if (N > 0) {
            int phi = 0;
            for (int x = 1; x <= N; x++) phi += gcd(x, N) == 1;
            phiSum += phi;

            int x = N, mu = 1;
            for (int p = 2; p * p <= x; p++) if (x % p == 0) {
                x /= p;
                mu = -mu;
                if (x % p == 0) mu = 0;
                while (x % p == 0) x /= p;
            }
            if (x > 1) mu = -mu;
            muSum += mu;

            for (int d = 1; d <= N; d++) if (N % d == 0) {
                divisorCountSum++;
                divisorSumSum += d;
            }
        }
        assert(multiplicativePrefixSum<ll>(N, phiPrimePrefix, phiPrimePower) == phiSum);
        assert(multiplicativePrefixSum<ll>(N, muPrimePrefix, muPrimePower) == muSum);
        assert(multiplicativePrefixSum<ll>(N, divisorCountPrimePrefix,
                                            divisorCountPrimePower) == divisorCountSum);
        assert(multiplicativePrefixSum<ll>(N, divisorSumPrimePrefix,
                                            divisorSumPrimePower) == divisorSumSum);
    }

    const int limit = 1000;
    vector<int> smallestPrime(limit + 1);
    vector<ll> unusualPrimePrefix(limit + 1), unusualPrefix(limit + 1);
    for (int p = 2; p <= limit; p++) {
        if (smallestPrime[p] == 0) {
            for (int x = p; x <= limit; x += p) {
                if (smallestPrime[x] == 0) smallestPrime[x] = p;
            }
            unusualPrimePrefix[p] = p + 3;
        }
        unusualPrimePrefix[p] += unusualPrimePrefix[p - 1];
    }
    unusualPrefix[1] = 1;
    for (int n = 2; n <= limit; n++) {
        int x = n;
        ll value = 1;
        while (x > 1) {
            int p = smallestPrime[x], e = 0;
            while (x % p == 0) {
                x /= p;
                e++;
            }
            value *= p + 3LL * e;
        }
        unusualPrefix[n] = unusualPrefix[n - 1] + value;
    }
    auto unusualPrimePower = [](ll p, int e) { return p + 3 * e; };
    for (int N = 0; N <= limit; N++) {
        auto unusualPrimeSum = [&](ll n) { return unusualPrimePrefix[n]; };
        assert(multiplicativePrefixSum<ll>(N, unusualPrimeSum,
                                            unusualPrimePower) == unusualPrefix[N]);
        auto modularPrimeSum = [&](ll n) { return modint998244353(unusualPrimePrefix[n]); };
        auto modularPrimePower = [&](ll p, int e) { return modint998244353(p + 3 * e); };
        assert(multiplicativePrefixSum<modint998244353>(N, modularPrimeSum,
                modularPrimePower) == modint998244353(unusualPrefix[N]));
    }
}
