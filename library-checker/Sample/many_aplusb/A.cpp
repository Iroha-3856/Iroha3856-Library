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

int main() {
    int T; cin >> T;
    while(T--) {
        ll A, B; cin >> A >> B;
        cout << A + B << endl;
    }
}