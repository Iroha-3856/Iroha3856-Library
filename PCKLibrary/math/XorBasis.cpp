//GF(2) 線形基底。insert / contains / maxXor は O(B^2)、kth は O(B)
//基底を reduced row echelon form に保ち、kth は表現可能値の昇順 0-indexed
template<class T = unsigned long long, int B = numeric_limits<T>::digits>
struct XorBasis {
    array<T, B> basis{};
    int rank = 0;

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
    bool contains(T x) const {
        for (int i = B - 1; i >= 0; i--) if (x >> i & 1) x ^= basis[i];
        return x == 0;
    }
    T maxXor(T x = 0) const {
        for (int i = B - 1; i >= 0; i--) x = max(x, x ^ basis[i]);
        return x;
    }
    T kth(unsigned long long k) const {
        assert(rank == 64 or k < (1ULL << rank));
        T ret = 0;
        int j = 0;
        for (int i = 0; i < B; i++) if (basis[i]) {
            if (k >> j & 1) ret ^= basis[i];
            j++;
        }
        return ret;
    }
};
