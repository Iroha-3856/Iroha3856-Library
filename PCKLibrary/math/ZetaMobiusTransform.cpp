// subset / divisor / multiple の zeta・Möbius 変換
// subset は O(N log N), divisor / multiple は O(N log log N) 程度
// 使いどころ: 全部分集合・全約数・全倍数からの和を一括計算し、必要なら包除で元へ戻す場合。
// 具体例: A={1, 2, 3, 4} を subsetZeta すると A[3]=1+2+3+4=10。mask 3 の全 submask の和である。
// 使い方:
// subsetZeta(A) は A[S]=sum_{T subset S} old[T] とし、subsetMobius(A) で元へ戻す。
// divisorZeta(A) は A[n]=sum_{d|n} old[d]、multipleZeta(A) は A[n]=sum_{n|k} old[k]。
// 対応する Mobius 関数が逆変換。subset 配列長は二冪、約数版は添字 0 を使わない。
// transformPrimes(N) は約数・倍数変換用に N 以下の素数を列挙する内部補助。
template<class T>
// A[S] を部分集合 T subset S に対する旧 A[T] の総和へ変換する。
void subsetZeta(vector<T>& A) {
    int N = (int)A.size();
    assert(N > 0 and (N & (N - 1)) == 0);
    for (int bit = 1; bit < N; bit <<= 1) {
        for (int s = 0; s < N; s++) if (s & bit) A[s] += A[s ^ bit];
    }
}

template<class T>
// subsetZeta の逆変換を行う。
void subsetMobius(vector<T>& A) {
    int N = (int)A.size();
    assert(N > 0 and (N & (N - 1)) == 0);
    for (int bit = 1; bit < N; bit <<= 1) {
        for (int s = 0; s < N; s++) if (s & bit) A[s] -= A[s ^ bit];
    }
}

// 約数・倍数変換で使う N 以下の素数を Eratosthenes 法で返す。
vector<int> transformPrimes(int N) {
    vector<bool> composite(N + 1);
    vector<int> primes;
    for (int p = 2; p <= N; p++) if (!composite[p]) {
        primes.push_back(p);
        if ((ll)p * p <= N) for (int q = p * p; q <= N; q += p) composite[q] = true;
    }
    return primes;
}

template<class T>
// A[n] を n の正の約数 d に対する旧 A[d] の総和へ変換する。
void divisorZeta(vector<T>& A) {
    int N = (int)A.size() - 1;
    for (int p : transformPrimes(N)) for (int i = 1; i * p <= N; i++) A[i * p] += A[i];
}

template<class T>
// divisorZeta の逆変換を行う。
void divisorMobius(vector<T>& A) {
    int N = (int)A.size() - 1;
    for (int p : transformPrimes(N)) for (int i = N / p; i >= 1; i--) A[i * p] -= A[i];
}

template<class T>
// A[n] を n の正の倍数 k に対する旧 A[k] の総和へ変換する。
void multipleZeta(vector<T>& A) {
    int N = (int)A.size() - 1;
    for (int p : transformPrimes(N)) for (int i = N / p; i >= 1; i--) A[i] += A[i * p];
}

template<class T>
// multipleZeta の逆変換を行う。
void multipleMobius(vector<T>& A) {
    int N = (int)A.size() - 1;
    for (int p : transformPrimes(N)) for (int i = 1; i * p <= N; i++) A[i] -= A[i * p];
}
