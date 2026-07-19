#include <bits/stdc++.h>
using namespace std;

#define rep(i, l, r) for (int i = (l); i < (r); i++)
#define ll long long
#define all(x) (x).begin(), (x).end()
#define siz(x) (int)x.size()

template<class T> bool chmin(T& a, T b) { if (a > b) { a = b; return 1; } return 0; }
template<class T> bool chmax(T& a, T b) { if (a < b) { a = b; return 1; } return 0; }

const int inf = 1e9;
const ll INF = 4e18;

/*
商列挙
返り値に含まれる {l, r, x} は、l <= i < r について、N/i = x であることを示す。
返り値は l の昇順 = x の降順
x = 0 は返り値に含まれないことに注意
verify: https://judge.yosupo.jp/submission/364489
        https://atcoder.jp/contests/abc239/submissions/74721536
*/
template<class T> 
vector<tuple<T, T, T>> enumerateQuotients(T N) {
    vector<tuple<T, T, T>> ret;
    T M;
    for (M = 1; M * M <= N; M++) {
        ret.push_back({M, M + 1, N / M});
    }
    for (int i = M; i >= 1; i--) {
        T L = N / (i + 1) + 1, R = N / i + 1;
        if (L < R and get<1>(ret.back()) <= L) ret.push_back({L, R, i});
    }
    return ret;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);

    ll N; cin >> N;
    vector<tuple<ll, ll, ll>> ans = enumerateQuotients(N);
    vector<ll> fans;
    for (auto[l, r, x] : ans) fans.push_back(x);
    reverse(all(fans));
    cout << siz(fans) << endl;
    for (ll a : fans) cout << a << " ";
    cout << endl;
}