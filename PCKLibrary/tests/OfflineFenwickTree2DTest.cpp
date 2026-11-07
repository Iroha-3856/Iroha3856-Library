// g++ -std=gnu++17 -O2 -Wall -Wextra -Werror PCKLibrary/tests/OfflineFenwickTree2DTest.cpp -o /tmp/offline_2d_bit_test && /tmp/offline_2d_bit_test
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "../data_structure/OfflineFenwickTree2D.cpp"

int main() {
    // Empty construction and empty / nonoverlapping queries.
    {
        OfflineFenwickTree2D<ll> bit({});
        assert(bit.prefixSum(0, 0) == 0);
        assert(bit.sum(-100, 100, -100, 100) == 0);
        assert(bit.sum(3, 3, 7, 8) == 0);
        assert(bit.sum(3, 8, 7, 7) == 0);
    }
    // Duplicate registrations, negative coordinates, negative updates and half-open boundaries.
    {
        OfflineFenwickTree2D<ll> bit({{2, 3}, {2, 3}, {5, 7}, {-4, 7}, {-4, -2}});
        bit.add(2, 3, 4);
        bit.add(2, 3, 7);
        bit.add(5, 7, 6);
        bit.add(-4, 7, -10);
        bit.add(-4, -2, 3);
        assert(bit.sum(2, 6, 3, 8) == 17);
        assert(bit.sum(2, 5, 3, 8) == 11);
        assert(bit.sum(5, 6, 3, 7) == 0);
        assert(bit.sum(-4, 3, -2, 8) == 4);
        assert(bit.sum(-4, 6, -2, 8) == 10);
        assert(bit.sum(-4, 6, 8, 9) == 0);
        assert(bit.prefixSum(6, 8) == 10);
        assert(bit.prefixSum(5, 7) == 14);
        bit.add(2, 3, -11);
        assert(bit.sum(2, 6, 3, 8) == 6);
    }
    // Full signed 64-bit coordinates: never add 1 to query endpoints.
    {
        constexpr ll LO = LLONG_MIN, HI = LLONG_MAX;
        OfflineFenwickTree2D<ll> bit({{LO, LO}, {LO, HI}, {HI, LO}, {HI, HI}, {0, 0}});
        bit.add(LO, LO, 3);
        bit.add(LO, HI, -7);
        bit.add(HI, LO, 11);
        bit.add(HI, HI, 13);
        bit.add(0, 0, 5);
        assert(bit.prefixSum(HI, HI) == 8); // (LO,LO) and (0,0); x or y == HI excluded
        assert(bit.sum(LO, HI, LO, HI) == 8);
        assert(bit.sum(LO, LO + 1, LO, HI) == 3);
        assert(bit.sum(LO, HI, HI, HI) == 0);
        assert(bit.sum(HI, HI, LO, HI) == 0);
        assert(bit.sum(HI - 1, HI, LO, HI) == 0);
        assert(bit.sum(0, 1, 0, 1) == 5);
    }
    mt19937_64 rng(20261106);
    auto randomCoordinate = [&]() -> ll {
        switch (rng() % 9) {
            case 0: return LLONG_MIN;
            case 1: return LLONG_MIN + (ll)(rng() % 5);
            case 2: return LLONG_MAX;
            case 3: return LLONG_MAX - (ll)(rng() % 5);
            default: return (ll)(rng() % 301) - 150;
        }
    };
    // Differential test against brute-force stored point weights.
    long long tested = 0;
#ifdef _GLIBCXX_DEBUG
    const int trialCount = 16, queryCount = 120, stressSize = 1800;
#else
    const int trialCount = 250, queryCount = 480, stressSize = 25000;
#endif
    for (int trial = 0; trial < trialCount; trial++) {
        int n = (int)(rng() % 100);
        vector<pair<ll, ll>> P(n);
        for (auto& p : P) p = {randomCoordinate(), randomCoordinate()};
        if (n >= 2 and trial % 2 == 0) P[1] = P[0];
        OfflineFenwickTree2D<ll> bit(P);
        sort(P.begin(), P.end());
        P.erase(unique(P.begin(), P.end()), P.end());
        vector<ll> value(P.size(), 0);
        auto expectedPrefix = [&](ll x, ll y) {
            ll ret = 0;
            for (int i = 0; i < (int)P.size(); i++) {
                if (P[i].first < x and P[i].second < y) ret += value[i];
            }
            return ret;
        };
        auto expectedSum = [&](ll lx, ll rx, ll ly, ll ry) {
            ll ret = 0;
            for (int i = 0; i < (int)P.size(); i++) {
                auto [x, y] = P[i];
                if (lx <= x and x < rx and ly <= y and y < ry) ret += value[i];
            }
            return ret;
        };
        for (int q = 0; q < queryCount; q++) {
            if (!P.empty() and rng() % 3 != 0) {
                int p = (int)(rng() % P.size());
                ll delta = (ll)(rng() % 2001) - 1000;
                bit.add(P[p].first, P[p].second, delta);
                value[p] += delta;
            } else {
                ll lx = randomCoordinate(), rx = randomCoordinate();
                ll ly = randomCoordinate(), ry = randomCoordinate();
                if (lx > rx) swap(lx, rx);
                if (ly > ry) swap(ly, ry);
                assert(bit.sum(lx, rx, ly, ry) == expectedSum(lx, rx, ly, ry));
                assert(bit.prefixSum(rx, ry) == expectedPrefix(rx, ry));
                assert(bit.sum(lx, lx, ly, ry) == 0);
                assert(bit.sum(lx, rx, ly, ly) == 0);
                tested++;
            }
        }
        // Force a final exhaustive sweep on small grids.
        if (trial % 20 == 0) {
            for (ll lx = -4; lx <= 4; lx++) for (ll rx = lx; rx <= 4; rx++) {
                for (ll ly = -4; ly <= 4; ly++) for (ll ry = ly; ry <= 4; ry++) {
                    assert(bit.sum(lx, rx, ly, ry) == expectedSum(lx, rx, ly, ry));
                    tested++;
                }
            }
        }
    }
    // A larger sparse update-only workload to exercise x/y compression.
    {
        const int n = stressSize;
        vector<pair<ll, ll>> P(n);
        for (int i = 0; i < n; i++) P[i] = {3LL * i, 7LL * ((i * 739) % n)};
        OfflineFenwickTree2D<ll> bit(P);
        ll total = 0;
        for (int i = 0; i < n; i++) {
            ll delta = i % 17 - 8;
            bit.add(P[i].first, P[i].second, delta);
            total += delta;
        }
        assert(bit.sum(LLONG_MIN, LLONG_MAX, LLONG_MIN, LLONG_MAX) == total);
        assert(bit.sum(1, 1, -10, 10) == 0);
    }
    cout << "OfflineFenwickTree2D tests passed (" << tested << " random rectangles)\n";
}
