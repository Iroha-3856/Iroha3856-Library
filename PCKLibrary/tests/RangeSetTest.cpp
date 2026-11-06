// g++ -std=gnu++17 -O2 -Wall -Wextra -Werror PCKLibrary/tests/RangeSetTest.cpp -o /tmp/rangeset_test && /tmp/rangeset_test
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "../data_structure/RangeSet.cpp"

void checkIntervals(const RangeSet& rs, const vector<pair<ll, ll>>& expected) {
    vector<pair<ll, ll>> actual(rs.intervals().begin(), rs.intervals().end());
    assert(actual == expected);
    assert(rs.size() == (int)expected.size());
    assert(rs.empty() == expected.empty());
    __int128_t len = 0;
    for (auto [l, r] : actual) len += (__int128_t)r - l;
    assert(rs.coveredLength() == len);
    for (int i = 1; i < (int)actual.size(); i++) {
        assert(actual[i - 1].second < actual[i].first);
    }
}

int main() {
    // 空区間、隣接する区間の統合、重複追加、穴あけ、完全削除。
    {
        RangeSet rs;
        checkIntervals(rs, {});
        assert(rs.mex(-7) == -7);
        assert(!rs.contains(-7));
        assert(rs.contains(0, 0));
        rs.insert(2, 2);
        rs.erase(0, 0);
        checkIntervals(rs, {});
        rs.insert(2, 5);
        rs.insert(5, 8);
        rs.insert(3, 6);
        checkIntervals(rs, {{2, 8}});
        assert(rs.contains(2, 8));
        assert(rs.contains(7));
        assert(!rs.contains(8));
        assert(!rs.contains(1, 3));
        assert(rs.mex(2) == 8);
        assert(rs.mex(8) == 8);
        rs.erase(4, 6);
        checkIntervals(rs, {{2, 4}, {6, 8}});
        assert(rs.mex(2) == 4);
        assert(rs.mex(6) == 8);
        assert(!rs.contains(2, 8));
        rs.erase(1, 3);
        checkIntervals(rs, {{3, 4}, {6, 8}});
        rs.erase(3, 7);
        checkIntervals(rs, {{7, 8}});
        rs.erase(-100, 100);
        checkIntervals(rs, {});
        rs.insert(-1, 0);
        rs.insert(0, 1);
        checkIntervals(rs, {{-1, 1}});
    }
    // ll の極限、幅 2^64-1 の被覆長、境界での mex。
    {
        RangeSet rs;
        rs.insert(LLONG_MIN, LLONG_MAX);
        checkIntervals(rs, {{LLONG_MIN, LLONG_MAX}});
        assert(rs.coveredLength() == (__int128_t)LLONG_MAX - LLONG_MIN);
        assert(rs.contains(LLONG_MIN));
        assert(rs.contains(LLONG_MAX - 1));
        assert(!rs.contains(LLONG_MAX));
        assert(rs.mex(LLONG_MIN) == LLONG_MAX);
        assert(rs.mex(LLONG_MAX) == LLONG_MAX);
        rs.erase(-1, 1);
        checkIntervals(rs, {{LLONG_MIN, -1}, {1, LLONG_MAX}});
        assert(rs.mex(LLONG_MIN) == -1);
        assert(rs.mex(1) == LLONG_MAX);
        rs.erase(LLONG_MIN, LLONG_MAX);
        checkIntervals(rs, {});
        rs.insert(LLONG_MAX - 1, LLONG_MAX);
        rs.insert(LLONG_MIN, LLONG_MIN + 1);
        checkIntervals(rs, {{LLONG_MIN, LLONG_MIN + 1}, {LLONG_MAX - 1, LLONG_MAX}});
        rs.erase(LLONG_MAX - 1, LLONG_MAX);
        rs.erase(LLONG_MIN, LLONG_MIN + 1);
        checkIntervals(rs, {});
    }
    // 多数の相離れた区間をまとめて統合し、端だけを残す削除を行う。
    {
        RangeSet rs;
        for (int i = 0; i < 5000; i++) rs.insert(3LL * i, 3LL * i + 1);
        assert(rs.size() == 5000);
        assert(rs.coveredLength() == 5000);
        rs.insert(0, 15000);
        checkIntervals(rs, {{0, 15000}});
        rs.erase(1, 14999);
        checkIntervals(rs, {{0, 1}, {14999, 15000}});
        rs.insert(1, 14999);
        checkIntervals(rs, {{0, 15000}});
    }
    mt19937_64 rng(20261106);
    // 小さな整数座標では bit 配列で全操作を愚直検証する。
    constexpr int LO = -90, HI = 110, N = HI - LO;
    for (int trial = 0; trial < 160; trial++) {
        RangeSet rs;
        vector<bool> covered(N, false);
        for (int q = 0; q < 600; q++) {
            ll l = LO + (ll)(rng() % (N + 1));
            ll r = LO + (ll)(rng() % (N + 1));
            if (l > r) swap(l, r);
            if (rng() & 1) {
                rs.insert(l, r);
                for (ll x = l; x < r; x++) covered[x - LO] = true;
            } else {
                rs.erase(l, r);
                for (ll x = l; x < r; x++) covered[x - LO] = false;
            }
            vector<pair<ll, ll>> expected;
            __int128_t count = 0;
            for (int i = 0; i < N;) {
                if (!covered[i]) {
                    i++;
                    continue;
                }
                int j = i;
                while (j < N and covered[j]) j++;
                expected.emplace_back(LO + i, LO + j);
                count += j - i;
                i = j;
            }
            checkIntervals(rs, expected);
            assert(rs.coveredLength() == count);
            // 全点の照合を定期的に行い、他の操作でも複数の点を抽出する。
            for (int i = 0; i < N; i++) {
                if (q % 29 != 0 and (i + q) % 23 != 0) continue;
                assert(rs.contains(LO + i) == covered[i]);
                int j = i;
                while (j < N and covered[j]) j++;
                assert(rs.mex(LO + i) == LO + j);
            }
            for (int repeat = 0; repeat < 5; repeat++) {
                int a = rng() % (N + 1), b = rng() % (N + 1);
                if (a > b) swap(a, b);
                bool full = true;
                for (int i = a; i < b; i++) full &= covered[i];
                assert(rs.contains(LO + a, LO + b) == full);
            }
        }
    }
    // 離れた極端な座標間でも順序と長さを正しく処理する。
    {
        vector<ll> xs = {LLONG_MIN, LLONG_MIN + 1, LLONG_MIN + 8,
                         -1000000000000LL, -7, -1, 0, 1, 7,
                         1000000000000LL, LLONG_MAX - 8, LLONG_MAX - 1, LLONG_MAX};
        RangeSet rs;
        vector<bool> covered(xs.size() - 1, false);
        for (int q = 0; q < 20000; q++) {
            int l = rng() % xs.size(), r = rng() % xs.size();
            if (l > r) swap(l, r);
            if (rng() & 1) {
                rs.insert(xs[l], xs[r]);
                for (int i = l; i < r; i++) covered[i] = true;
            } else {
                rs.erase(xs[l], xs[r]);
                for (int i = l; i < r; i++) covered[i] = false;
            }
            vector<pair<ll, ll>> expected;
            __int128_t length = 0;
            for (int i = 0; i < (int)covered.size();) {
                if (!covered[i]) {
                    i++;
                    continue;
                }
                int j = i;
                while (j < (int)covered.size() and covered[j]) {
                    length += (__int128_t)xs[j + 1] - xs[j];
                    j++;
                }
                expected.emplace_back(xs[i], xs[j]);
                i = j;
            }
            checkIntervals(rs, expected);
            assert(rs.coveredLength() == length);
            for (int i = 0; i < (int)covered.size(); i++) {
                assert(rs.contains(xs[i]) == covered[i]);
                int j = i;
                while (j < (int)covered.size() and covered[j]) j++;
                assert(rs.mex(xs[i]) == xs[j]);
            }
        }
    }
    cout << "RangeSet tests passed\n";
}
