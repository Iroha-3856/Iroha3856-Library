// mod は素数
// 使いどころ: 加減乗除を常に同じ素数 mod で行い、負数の正規化や逆元計算を式から隠したい場合。
// 具体例: modint998244353 x=-1 とすると x.value=998244352、(x+2).value=1。
// 使い方:
// using mint = modint998244353; mint x = -1; のように整数から自動で [0, mod) へ正規化する。
// +, -, *, /, pow(n), inv() が使え、出力する整数値は x.value。除算の分母は 0 でないこと。
// 別の素数法では using mint = Modint<MOD>; とする。pow の指数は非負。
template<int mod>
struct Modint {
    int value;

    // 整数 x を [0, mod) に正規化して保持する。
    Modint(long long x = 0) {
        value = (int)(x % mod);
        if (value < 0) value += mod;
    }
    // 自身の非負整数 n 乗を返す。
    Modint pow(long long n) const {
        assert(n >= 0);
        Modint ret = 1, x = *this;
        while (n) {
            if (n & 1) ret *= x;
            x *= x;
            n >>= 1;
        }
        return ret;
    }
    // Fermat の小定理で乗法逆元を返す。value!=0 が必要。
    Modint inv() const {
        assert(value != 0);
        return pow(mod - 2);
    }
    // x を加え、正規化した自身を返す。
    Modint& operator+=(Modint x) {
        ll sum = (ll)value + x.value;
        value = (int)(sum >= mod ? sum - mod : sum);
        return *this;
    }
    // x を引き、正規化した自身を返す。
    Modint& operator-=(Modint x) {
        value -= x.value;
        if (value < 0) value += mod;
        return *this;
    }
    // x を掛け、正規化した自身を返す。
    Modint& operator*=(Modint x) {
        value = (int)((ll)value * x.value % mod);
        return *this;
    }
    // x の逆元を掛け、正規化した自身を返す。
    Modint& operator/=(Modint x) {
        return *this *= x.inv();
    }
    // 二つの値の和を返す。
    friend Modint operator+(Modint x, Modint y) { return x += y; }
    // 二つの値の差を返す。
    friend Modint operator-(Modint x, Modint y) { return x -= y; }
    // 二つの値の積を返す。
    friend Modint operator*(Modint x, Modint y) { return x *= y; }
    // 左辺を右辺で割った値を返す。
    friend Modint operator/(Modint x, Modint y) { return x /= y; }
    // 正規化済みの値が等しいかを返す。
    friend bool operator==(Modint x, Modint y) { return x.value == y.value; }
    // 正規化済みの値が異なるかを返す。
    friend bool operator!=(Modint x, Modint y) { return !(x == y); }
};

using modint1000000007 = Modint<1000000007>;
using modint998244353 = Modint<998244353>;
