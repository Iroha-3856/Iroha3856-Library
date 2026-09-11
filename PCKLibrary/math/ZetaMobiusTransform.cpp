//subset / divisor / multiple の zeta・Möbius 変換
//subset は O(N log N), divisor / multiple は O(N log log N) 程度
template<class T>
void subsetZeta(vector<T>& A) {
    int N = (int)A.size();
    assert(N > 0 and (N & (N - 1)) == 0);
    for (int bit = 1; bit < N; bit <<= 1) {
        for (int s = 0; s < N; s++) if (s & bit) A[s] += A[s ^ bit];
    }
}

template<class T>
void subsetMobius(vector<T>& A) {
    int N = (int)A.size();
    assert(N > 0 and (N & (N - 1)) == 0);
    for (int bit = 1; bit < N; bit <<= 1) {
        for (int s = 0; s < N; s++) if (s & bit) A[s] -= A[s ^ bit];
    }
}

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
void divisorZeta(vector<T>& A) {
    int N = (int)A.size() - 1;
    for (int p : transformPrimes(N)) for (int i = 1; i * p <= N; i++) A[i * p] += A[i];
}

template<class T>
void divisorMobius(vector<T>& A) {
    int N = (int)A.size() - 1;
    for (int p : transformPrimes(N)) for (int i = N / p; i >= 1; i--) A[i * p] -= A[i];
}

template<class T>
void multipleZeta(vector<T>& A) {
    int N = (int)A.size() - 1;
    for (int p : transformPrimes(N)) for (int i = N / p; i >= 1; i--) A[i] += A[i * p];
}

template<class T>
void multipleMobius(vector<T>& A) {
    int N = (int)A.size() - 1;
    for (int p : transformPrimes(N)) for (int i = 1; i * p <= N; i++) A[i] -= A[i * p];
}
