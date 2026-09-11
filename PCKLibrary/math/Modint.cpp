//mod は素数
template<int mod>
struct Modint {
    int value;

    Modint(long long x = 0) {
        value = (int)(x % mod);
        if (value < 0) value += mod;
    }
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
    Modint inv() const {
        assert(value != 0);
        return pow(mod - 2);
    }
    Modint& operator+=(Modint x) {
        value += x.value;
        if (value >= mod) value -= mod;
        return *this;
    }
    Modint& operator-=(Modint x) {
        value -= x.value;
        if (value < 0) value += mod;
        return *this;
    }
    Modint& operator*=(Modint x) {
        value = (int)((ll)value * x.value % mod);
        return *this;
    }
    Modint& operator/=(Modint x) {
        return *this *= x.inv();
    }
    friend Modint operator+(Modint x, Modint y) { return x += y; }
    friend Modint operator-(Modint x, Modint y) { return x -= y; }
    friend Modint operator*(Modint x, Modint y) { return x *= y; }
    friend Modint operator/(Modint x, Modint y) { return x /= y; }
    friend bool operator==(Modint x, Modint y) { return x.value == y.value; }
    friend bool operator!=(Modint x, Modint y) { return !(x == y); }
};

using modint1000000007 = Modint<1000000007>;
using modint998244353 = Modint<998244353>;
