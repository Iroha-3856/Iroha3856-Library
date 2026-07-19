/*
https://qiita.com/drken/items/b97ff231e43bce50199a

返り値: a と b の最大公約数
ax + by = gcd(a, b) を満たす (x, y) が格納される

a <= 0, b <= 0 の場合でも動くが、返ってくる gcd は負になる可能性がある
gcd が 0 となるのは、 a = b = 0 の時のみ。
*/
long long extGCD(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long d = extGCD(b, a%b, y, x);
    y -= a/b * x;
    return d;
}