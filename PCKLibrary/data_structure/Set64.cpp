// 固定整数 universe [0, N) 上の 64 分木 set。構築 O(N / 64)、各操作 O(log_64 N)
// 各階層の 1 bit は、一つ下の階層の対応する 64 bit word が空でないことを表す。
// 使いどころ: 座標圧縮済み整数の集合で、挿入・削除と直前・直後の要素検索を高速に繰り返す場合。
// 具体例: 2, 5, 9 を insert すると lowerBound(4)=5、upperBound(5)=9、predecessor(4)=2。
// 使い方:
// Set64 set(N); set.insert(x), set.erase(x), set.contains(x) で集合を更新・参照する。
// lowerBound(x) は x 以上、upperBound(x) は x より大きい最小要素を返し、存在しなければ N。
// predecessor(x) は x 以下の最大要素を返し、存在しなければ -1。重複 insert は false を返す。
// std::set と異なり格納可能な値は構築時の [0, N) に限定されるが、各操作で比較や動的確保を行わない。
struct Set64 {
    int N, elementCount = 0;
    vector<vector<unsigned long long>> level;

    // 空の整数集合を作り、格納可能な値を 0 以上 n 未満に固定する。
    Set64(int n) : N(n) {
        assert(N >= 0);
        for (int size = N; size > 0;) {
            int wordCount = (int)(((ll)size + 63) >> 6);
            level.push_back(vector<unsigned long long>(wordCount));
            if (wordCount == 1) break;
            size = wordCount;
        }
    }

    // 現在格納されている相異なる整数の個数を返す。
    int size() const { return elementCount; }

    // 集合が空かを返す。
    bool empty() const { return elementCount == 0; }

    // x が集合に含まれるかを返す。範囲外の x は含まれないものとして false を返す。
    bool contains(int x) const {
        if (x < 0 or N <= x) return false;
        return level[0][x >> 6] >> (x & 63) & 1;
    }

    // x を追加する。新しく追加したなら true、既に存在したなら false を返す。
    bool insert(int x) {
        assert(0 <= x and x < N);
        if (contains(x)) return false;
        elementCount++;
        for (vector<unsigned long long>& bits : level) {
            bits[x >> 6] |= 1ULL << (x & 63);
            x >>= 6;
        }
        return true;
    }

    // x を削除する。削除したなら true、存在しなかったなら false を返す。
    bool erase(int x) {
        assert(0 <= x and x < N);
        if (!contains(x)) return false;
        elementCount--;
        for (vector<unsigned long long>& bits : level) {
            unsigned long long& word = bits[x >> 6];
            word &= ~(1ULL << (x & 63));
            if (word) break;
            x >>= 6;
        }
        return true;
    }

    // x 以上の最小要素を返す。存在しなければ番兵 N を返す。
    int lowerBound(int x) const {
        x = max(x, 0);
        if (N <= x) return N;
        for (int h = 0; h < (int)level.size(); h++) {
            if (x >> 6 >= (int)level[h].size()) return N;
            unsigned long long word = level[h][x >> 6] >> (x & 63);
            if (!word) {
                x = (x >> 6) + 1;
                continue;
            }
            x += __builtin_ctzll(word);
            for (int g = h - 1; g >= 0; g--) {
                unsigned long long lowerWord = level[g][x];
                x = (x << 6) + __builtin_ctzll(lowerWord);
            }
            return x < N ? x : N;
        }
        return N;
    }

    // x より大きい最小要素を返す。存在しなければ番兵 N を返す。
    int upperBound(int x) const {
        if (x >= N - 1) return N;
        return lowerBound(x + 1);
    }

    // x 以下の最大要素を返す。存在しなければ番兵 -1 を返す。
    int predecessor(int x) const {
        if (x < 0 or N == 0) return -1;
        x = min(x, N - 1);
        for (int h = 0; h < (int)level.size(); h++) {
            unsigned long long word = level[h][x >> 6] << (63 - (x & 63));
            if (!word) {
                x = (x >> 6) - 1;
                if (x < 0) return -1;
                continue;
            }
            x -= __builtin_clzll(word);
            for (int g = h - 1; g >= 0; g--) {
                unsigned long long lowerWord = level[g][x];
                x = (x << 6) + 63 - __builtin_clzll(lowerWord);
            }
            return x;
        }
        return -1;
    }

    // 集合の最小要素を返す。空集合なら番兵 N を返す。
    int minimum() const { return lowerBound(0); }

    // 集合の最大要素を返す。空集合なら番兵 -1 を返す。
    int maximum() const { return predecessor(N - 1); }
};
