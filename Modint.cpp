template<int mod>
class Modint {
public:
    int value;

    Modint() {value = 0;}
    template<class T> Modint(T x) { 
        value = x % mod; 
        if (value < 0) value += mod;
    }

    template<class T>
    Modint<mod> pow(T x) const {
        Modint<mod> ret = 1, mul = *this;
        while(x) {
            if (x&1) ret *= mul;
            mul *= mul;
            x>>=1;
        }
        return ret;
    }
    Modint<mod> inv() const {
        return Modint<mod>(value).pow(mod-2);
    }

    friend Modint<mod> operator+(const Modint<mod>& A, const Modint<mod>& B) {
        return (A.value + B.value) % mod;
    }
    friend Modint<mod> operator-(const Modint<mod>& A, const Modint<mod>& B) {
        return (A.value - B.value + mod) % mod;
    }
    friend Modint<mod> operator*(const Modint<mod>& A, const Modint<mod>& B) {
        return ((long long)A.value * B.value) % mod;
    }
    friend Modint<mod> operator/(const Modint<mod>& A, const Modint<mod>& B) {
        return A * B.inv();
    }

    //++a
    //前置インクリメント
    Modint<mod>& operator++() {
        value++;
        if (value == mod) value = 0;
        return *this;
    }
    Modint<mod>& operator--() {
        value--;
        if (value == -1) value = mod-1;
        return *this;
    }
    //後置インクリメント
    Modint<mod> operator++(int) {
        Modint<mod> ret = *this;
        ++*this;
        return ret;
    }
    Modint<mod> operator--(int) {
        Modint<mod> ret = *this;
        --*this;
        return ret;
    }

    Modint<mod>& operator+=(const Modint<mod> A) {
        *this = *this + A;
        return *this;
    }
    Modint<mod>& operator-=(const Modint<mod> A) {
        *this = *this - A;
        return *this;
    }
    Modint<mod>& operator*=(const Modint<mod> A) {
        *this = *this * A;
        return *this;
    }
    Modint<mod>& operator/=(const Modint<mod> A) {
        *this = *this / A;
        return *this;
    }

    Modint<mod> operator+() const {
        return *this;
    }
    Modint<mod> operator-() const {
        return Modint<mod>() - *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Modint<mod>& v) {
        os << v.value;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Modint<mod>& v) {
        is >> v.value;
        return is;
    }
};

using modint1000000007 = Modint<1000000007>;
using modint998244353 = Modint<998244353>;