//unsigned long long のオーバーフローを mod 2^64 として使う
//構築 O(N)、部分文字列 hash O(1)。衝突の可能性はある
struct RollingHash {
    const unsigned long long base = 1000003;
    vector<unsigned long long> hash, power;

    RollingHash(const string& S) : hash(S.size() + 1), power(S.size() + 1, 1) {
        for (int i = 0; i < (int)S.size(); i++) {
            hash[i + 1] = hash[i] * base + (unsigned char)S[i] + 1;
            power[i + 1] = power[i] * base;
        }
    }
    //S[l, r)
    unsigned long long get(int l, int r) const {
        return hash[r] - hash[l] * power[r - l];
    }
};
