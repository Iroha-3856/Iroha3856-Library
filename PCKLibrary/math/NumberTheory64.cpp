// 64 bit 決定的 Miller-Rabin と Pollard's rho。積は unsigned __int128
// factorize64(n) は素因数を昇順に重複込みで返す。期待 O(n^(1/4)) 程度
// 使いどころ: sqrt(n) までの試し割りが間に合わない 64 bit 整数の素数判定・素因数分解。
// 具体例: factorize64(84)={2, 2, 3, 7}、isPrime64(1000000007)=true。
// 使い方:
// bool prime = isPrime64(n); で unsigned 64 bit 整数を決定的に素数判定する。
// vector<u64> factors = factorize64(n); は n>=1 の素因数を重複込み昇順で返し、n==1 は空。
// modMultiply64 / modPower64 は積が 64 bit を超える法演算にも使える。mod>0 を満たすこと。
// Pollard's rho は乱択なので実行時間に揺れがあるが、返す因数分解結果自体は検算可能。
using u64 = unsigned long long;
using u128 = unsigned __int128;

// a*b mod mod を 128 bit 中間積で overflow せず返す。
u64 modMultiply64(u64 a, u64 b, u64 mod) { return (u64)((u128)a * b % mod); }

// a^n mod mod を二分累乗で返す。mod>0 が必要。
u64 modPower64(u64 a, u64 n, u64 mod) {
    u64 ret = 1 % mod;
    while (n) {
        if (n & 1) ret = modMultiply64(ret, a, mod);
        a = modMultiply64(a, a, mod);
        n >>= 1;
    }
    return ret;
}

// unsigned 64 bit 整数 n が素数かを決定的 Miller-Rabin で判定する。
bool isPrime64(u64 n) {
    if (n < 2) return false;
    for (u64 p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
        if (n % p == 0) return n == p;
    }
    u64 d = n - 1, s = 0;
    while ((d & 1) == 0) d >>= 1, s++;
    for (u64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
        if (a % n == 0) continue;
        u64 x = modPower64(a % n, d, n);
        if (x == 1 or x == n - 1) continue;
        bool composite = true;
        for (u64 r = 1; r < s; r++) {
            x = modMultiply64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

// 合成数 n の非自明因数を乱択 Pollard's rho で一つ返す。
u64 pollardRho(u64 n) {
    if (n % 2 == 0) return 2;
    static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    while (true) {
        u64 c = uniform_int_distribution<u64>(1, n - 1)(rng);
        u64 x = uniform_int_distribution<u64>(0, n - 1)(rng), y = x, d = 1;
        // Pollard's rho の疑似乱数写像 v -> v^2+c mod n。
        auto f = [&](u64 v) { return (u64)(((u128)modMultiply64(v, v, n) + c) % n); };
        while (d == 1) {
            x = f(x); y = f(f(y));
            d = gcd(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}

// n の素因数を再帰的に ret へ追加する内部関数。順序は保証しない。
void factorize64(u64 n, vector<u64>& ret) {
    if (n == 1) return;
    if (isPrime64(n)) return ret.push_back(n);
    u64 d = pollardRho(n);
    factorize64(d, ret);
    factorize64(n / d, ret);
}

// n の素因数を重複込み昇順で返す。n>=1 が必要。
vector<u64> factorize64(u64 n) {
    vector<u64> ret;
    factorize64(n, ret);
    sort(ret.begin(), ret.end());
    return ret;
}
