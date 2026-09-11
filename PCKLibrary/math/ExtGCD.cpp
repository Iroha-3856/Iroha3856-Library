/*
返り値: gcd(a, b)（常に非負）
ax + by = gcd(a, b) を満たす (x, y) が格納される
gcd が 0 となるのは a = b = 0 のときのみ。LLONG_MIN は与えない
計算量 O(log min(|a|, |b|))
*/
long long extGCD(long long a, long long b, long long& x, long long& y) {
    if (b == 0) {
        x = a < 0 ? -1 : 1;
        y = 0;
        return abs(a);
    }
    long long d = extGCD(b, a % b, y, x);
    y -= a / b * x;
    return d;
}
