// unsigned long long のオーバーフローを mod 2^64 として使う
// 構築 O(N)、部分文字列 hash O(1)。衝突の可能性はある
// 使いどころ: 多数の部分文字列一致や二分探索つき LCP を高速に行い、極小確率の衝突を許せる場合。
// 具体例: s="banana" なら get(1, 4)==get(3, 6)。どちらの半開区間も "ana" である。
// 使い方:
// RollingHash hs(s), ht(t); と構築し、hs.get(l, r)==ht.get(a, b) で同じ長さの部分文字列を比較する。
// get は 0-indexed 半開区間。異なるオブジェクトも base が同じなので直接比較できる。
// hash 一致は文字列一致を保証せず衝突可能性がある。厳密性が必要なら suffix array 等を使う。
struct RollingHash {
    const unsigned long long base = 1000003;
    vector<unsigned long long> hash, power;

    // 文字列 S の prefix hash と base の冪を前計算する。
    RollingHash(const string& S) : hash(S.size() + 1), power(S.size() + 1, 1) {
        for (int i = 0; i < (int)S.size(); i++) {
            hash[i + 1] = hash[i] * base + (unsigned char)S[i] + 1;
            power[i + 1] = power[i] * base;
        }
    }
    // 元文字列の半開区間 S[l, r) の位置非依存 hash を返す。
    unsigned long long get(int l, int r) const {
        return hash[r] - hash[l] * power[r - l];
    }
};
