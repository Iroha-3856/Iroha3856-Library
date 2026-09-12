// 拡張 Euclid。gcd(a, b) と、a*x+b*y=gcd(a, b) を満たす係数 x, y を O(log min(|a|, |b|)) で求める。
// 使いどころ: 一次不定方程式、線形合同式、法が素数とは限らないときの逆元計算。
// 具体例: extGCD(6, 9, x, y)=3 となり、一例として x=-1, y=1、6*(-1)+9*1=3 を得る。
// 使い方:
// long long x, y; long long g = extGCD(a, b, x, y); とする。返り値 g は常に非負。
// a*X+b*Y=c は c%g==0 のときだけ解を持ち、X=x*(c/g), Y=y*(c/g) が一つの解になる。
// gcd(a, m)==1 なら x を [0, m) に正規化した値が a の mod m 逆元。LLONG_MIN は与えない。
// a=b=0 の場合だけ g=0 となり、係数は意味のある一意解ではない。
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
