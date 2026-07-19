//素数判定
bool isPrime(long long N) {
    for (long long d = 2; d * d <= N; d++) {
        if (N%d == 0) return false;
    }
    return true;
}

//エラトステネスの篩による [1, N] の素数判定
vector<bool> primeSieve(int N) {
    vector<bool> isPrime(N+1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i <= N; i++) {
        if (isPrime[i]) {
            for (int j = 2*i; j <= N; j += i) isPrime[j] = false;
        }
    }
    return isPrime;
}

//最小素因数 (Least Prime Factor)
long long lpf(long long N) {
    for (long long d = 2; d * d <= N; d++) {
        if (N%d == 0) {
            return d;
        }
    }
    return N;
}

//エラトステネスの篩による [1, N] の最小素因数
vector<int> lpfSieve(int N) {
    vector<int> lpf(N+1);
    iota(lpf.begin(), lpf.end(), 0);
    lpf[0] = lpf[1] = -1;
    for (int i = 2; i <= N; i++) {
        if (lpf[i] == i) {
            for (int j = 2*i; j <= N; j += i) {
                lpf[j] = min(lpf[j], i);
            }
        }
    }
    return lpf;
}

//N の 素因数分解
//{p, e} of N を返す
vector<pair<long long, int>> primeFactorization(long long N) {
    vector<pair<long long, int>> ret;
    for (long long d = 2; d * d <= N; d++) {
        if (N%d) continue;
        int cnt = 0;
        while(N%d == 0) {
            cnt++;
            N /= d;
        }
        ret.push_back({d, cnt});
    }
    if (N != 1) ret.push_back({N, 1});
    return ret;
} 

//メビウス関数
int mobius(long long N) {
    if (N == 1) return 1;
    vector<pair<ll, int>> ret = primeFactorization(N);
    bool ok = true;
    for (auto[p, e] : ret) if (e != 1) ok = false;
    if (ok) {
        int t = ret.size()%2;
        if (t == 1) return -1;
        return 1;
    }
    return 0;
}

//[1, N] の素因数分解
//[1, N] の lpf 配列を与えて、{{p, e} of 0, {p, e} of 1, ... , {p, e} of N} を返す
//0, 1は空
vector<vector<pair<int, int>>> primeFactorization(vector<int> lpf) {
    int N = (int)lpf.size()-1;
    vector<vector<pair<int, int>>> ret(N+1);
    for (int i = 2; i <= N; i++) {
        if (lpf[i] == i) ret[i] = {{i, 1}};
        else {
            ret[i] = ret[i/lpf[i]];
            if (ret[i].back().first == lpf[i]) {
                ret[i].back().second++;
            }
            else ret[i].push_back({lpf[i], 1});
        }
    }
    return ret;
}