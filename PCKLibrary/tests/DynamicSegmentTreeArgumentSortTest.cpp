// g++ -std=gnu++17 -O2 -Wall -Wextra -Werror -fsanitize=address,undefined -fno-omit-frame-pointer
//     PCKLibrary/tests/DynamicSegmentTreeArgumentSortTest.cpp -o /tmp/pck_test && /tmp/pck_test
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "../data_structure/DynamicSegmentTree.cpp"
#include "../geometry/Geometry2D_integer.cpp"
#include "../geometry/ArgumentSort.cpp"

ll sum(ll a, ll b) { return a + b; }
ll zero() { return 0; }
string concatenate(string a, string b) { return a + b; }
string emptyString() { return ""; }

int main() {
    using SumTree = DynamicSegmentTree<ll, sum, zero>;
    mt19937_64 rng(20261106);
    // Empty tree, empty intervals, coordinate boundaries, signed 64-bit domain.
    {
        SumTree seg(LLONG_MIN, LLONG_MAX);
        assert(seg.node.empty());
        assert(seg.allProd() == 0);
        assert(seg.prod(LLONG_MIN, LLONG_MAX) == 0);
        assert(seg.prod(7, 7) == 0);
        seg.set(LLONG_MIN, 3);
        seg.set(LLONG_MAX - 1, 5);
        seg.apply(-1, -2);
        seg.apply(0, 9);
        seg.apply(LLONG_MIN, 7);
        assert(seg.get(LLONG_MIN) == 10);
        assert(seg.get(LLONG_MIN + 1) == 0);
        assert(seg.get(-1) == -2);
        assert(seg.get(0) == 9);
        assert(seg.get(LLONG_MAX - 1) == 5);
        assert(seg.prod(LLONG_MIN, 0) == 8);
        assert(seg.prod(0, LLONG_MAX) == 14);
        assert(seg.allProd() == 22);
        seg.set(LLONG_MIN, 0);
        seg.set(LLONG_MAX - 1, 0);
        assert(seg.allProd() == 7);
    }
    // Differential randomized range-sum test (set/apply/get/prod/allProd).
    for (int trial = 0; trial < 180; trial++) {
        int n = 1 + rng() % 85;
        ll offset = (ll)(rng() % 200) - 100;
        SumTree seg(offset, offset + n);
        vector<ll> a(n);
        for (int q = 0; q < 700; q++) {
            int type = rng() % 5;
            int p = rng() % n;
            ll x = (ll)(rng() % 1001) - 500;
            if (type == 0) {
                seg.set(offset + p, x);
                a[p] = x;
            } else if (type == 1) {
                seg.apply(offset + p, x);
                a[p] += x;
            } else if (type == 2) {
                assert(seg.get(offset + p) == a[p]);
            } else {
                int l = rng() % (n + 1), r = rng() % (n + 1);
                if (l > r) swap(l, r);
                ll expected = accumulate(a.begin() + l, a.begin() + r, 0LL);
                assert(seg.prod(offset + l, offset + r) == expected);
            }
            assert(seg.allProd() == accumulate(a.begin(), a.end(), 0LL));
        }
    }
    // Noncommutative monoid: range concatenation must preserve left-to-right order.
    {
        DynamicSegmentTree<string, concatenate, emptyString> seg(-8, 17);
        vector<string> a(25);
        for (int q = 0; q < 5000; q++) {
            int p = rng() % 25;
            string x(1, 'a' + (rng() % 26));
            if (rng() % 2) {
                seg.set(p - 8, x);
                a[p] = x;
            } else {
                seg.apply(p - 8, x);
                a[p] += x;
            }
            int l = rng() % 26, r = rng() % 26;
            if (l > r) swap(l, r);
            string expected;
            for (int i = l; i < r; i++) expected += a[i];
            assert(seg.prod(l - 8, r - 8) == expected);
        }
    }
    // Axes, zero vectors, duplicates and equal-angle distance tie-break.
    {
        vector<Point> p = {{0, -1}, {-1, 0}, {0, 0}, {0, 2}, {2, 0},
                           {-2, 0}, {0, 1}, {1, 0}, {0, 0}};
        vector<Point> expected = {{0, 0}, {0, 0}, {1, 0}, {2, 0}, {0, 1},
                                  {0, 2}, {-1, 0}, {-2, 0}, {0, -1}};
        assert(argumentSort(p) == expected);
        assert(argumentSort({}).empty());
        assert(argumentSort({Point(0, 0)}) == vector<Point>({Point(0, 0)}));
    }
    // Independent atan2l oracle on small integer coordinates.
    auto key = [](Point p) -> long double {
        if (p.x == 0 and p.y == 0) return -1;
        long double angle = atan2l((long double)p.y, (long double)p.x);
        if (angle < 0) angle += 2 * acosl(-1.L);
        return angle;
    };
    for (int trial = 0; trial < 3000; trial++) {
        int n = rng() % 130;
        vector<Point> p(n);
        for (Point& v : p) {
            v.x = (ll)(rng() % 201) - 100;
            v.y = (ll)(rng() % 201) - 100;
        }
        vector<Point> expected = p;
        sort(expected.begin(), expected.end(), [&](Point a, Point b) {
            long double aa = key(a), bb = key(b);
            if (fabsl(aa - bb) > 1e-14L) return aa < bb;
            __int128_t da = (__int128_t)a.x * a.x + (__int128_t)a.y * a.y;
            __int128_t db = (__int128_t)b.x * b.x + (__int128_t)b.y * b.y;
            return da < db;
        });
        assert(argumentSort(p) == expected);
    }
    // Large integer vectors: avoid 64-bit cross-product overflow and check ordering.
    {
        vector<Point> p = {{1000000000000LL, 1000000000000LL - 1},
                           {1000000000000LL - 1, 1000000000000LL},
                           {-1000000000000LL, -1000000000000LL},
                           {0, 0}, {1000000000000LL, 0}};
        auto q = argumentSort(p);
        assert(q[0] == Point(0, 0));
        assert(q[1] == Point(1000000000000LL, 0));
        assert(q[2] == p[0]);
        assert(q[3] == p[1]);
        assert(q[4] == p[2]);
    }
    cout << "DynamicSegmentTree / ArgumentSort tests passed\n";
}
