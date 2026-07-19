//ull -> T の HashMap
//暗黙に型キャストが起こるが問題なく、ll -> T として使うこともできるはず
//verify: https://judge.yosupo.jp/submission/364422
template<class T>
struct HashMap {
    using ull = unsigned long long;
    int mask;
    vector<ull> key;
    vector<T> val;
    vector<bool> used;

    HashMap(int N = 0) {
        int n = 1;
        while(n < 2 * N) n <<= 1;
        key.resize(n); val.resize(n); used.resize(n, false);
        mask = n - 1;
    }
    
    int hash(ull x) {
        static const ull FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
        x += FIXED_RANDOM;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return (x ^ (x >> 31)) & mask;
    }

    int index(ull x) {
        int i = hash(x);
        while(used[i] and key[i] != x) i = (i + 1) & mask;
        return i;
    }

    T get(ull x, T default_value) {
        int i = index(x);
        if (not used[i]) return default_value;
        return val[i];
    }
    void set(ull x, T v) {
        int i = index(x);
        if (not used[i]) {
            key[i] = x; used[i] = true;
        }
        val[i] = v;
    }
};