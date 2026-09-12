// GF(2) 線形基底。insert / contains / maxXor / kth は O(B)
// 基底を reduced row echelon form に保ち、kth は表現可能値の昇順 0-indexed
// 使いどころ: 与えられた数の部分集合 XOR が作れるか、または作れる値の最大・k 番目を求める場合。
// 具体例: 1, 2 を insert すると表現可能値は 0, 1, 2, 3 で、kth(2)=2、maxXor()=3。
// 使い方:
// XorBasis<unsigned long long> basis; basis.insert(x); で XOR 線形空間へ追加する。
// contains(x) は表現可能性、maxXor(seed) は seed と表現可能値の XOR の最大値。
// kth(k) は相異なる表現可能値の k 番目。0 <= k < 2^rank を満たすこと。
// insert が false なら x は既存基底に従属。重複する部分集合の個数は管理しない。
template<class T = unsigned long long, int B = numeric_limits<T>::digits>
struct XorBasis {
    array<T, B> basis{};
    int rank = 0;

    // x を基底へ挿入する。線形独立なら true、既存基底で表現可能なら false。
    bool insert(T x) {
        for (int i = B - 1; i >= 0; i--) if (x >> i & 1) {
            if (basis[i]) x ^= basis[i];
            else {
                basis[i] = x;
                for (int j = 0; j < i; j++) if (basis[i] >> j & 1) basis[i] ^= basis[j];
                for (int j = i + 1; j < B; j++) if (basis[j] >> i & 1) basis[j] ^= basis[i];
                rank++;
                return true;
            }
        }
        return false;
    }
    // x が基底の XOR 線形結合で表現できるかを返す。
    bool contains(T x) const {
        for (int i = B - 1; i >= 0; i--) if (x >> i & 1) x ^= basis[i];
        return x == 0;
    }
    // 初期値 x と表現可能な値との XOR の最大値を返す。
    T maxXor(T x = 0) const {
        for (int i = B - 1; i >= 0; i--) x = max(x, x ^ basis[i]);
        return x;
    }
    // 表現可能な相異なる値を昇順に見た 0-indexed k 番目を返す。k は範囲内とする。
    T kth(unsigned long long k) const {
        assert(rank >= 64 or k < (1ULL << rank));
        T ret = 0;
        int j = 0;
        for (int i = 0; i < B; i++) if (basis[i]) {
            if (k >> j & 1) ret ^= basis[i];
            j++;
        }
        return ret;
    }
};
