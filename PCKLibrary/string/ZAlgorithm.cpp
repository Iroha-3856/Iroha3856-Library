// Z[i] = S と S[i, N) の最長共通接頭辞の長さ。O(N)
// 使いどころ: 文字列の各位置と先頭との一致長を一括計算し、pattern 検索や周期判定へ使う場合。
// 具体例: zAlgorithm("aaaa")={4, 3, 2, 1}。位置 i から末尾まで先頭と一致する。
// 使い方:
// vector<int> z = zAlgorithm(S); とすると z[0]=|S|、z[i] が先頭との一致長。
// パターン検索は pattern + 番兵 + text を渡し、text 側で z[i]>=|pattern| の位置を拾う。
// string 以外の vector にも使える。番兵は入力に現れない要素を選ぶ。
template<class T>
// 各 i について S と S[i, N) の最長共通接頭辞長を返す。
vector<int> zAlgorithm(const T& S) {
    int n = (int)S.size();
    if (n == 0) return {};
    vector<int> Z(n);
    Z[0] = n;
    for (int i = 1, j = 0; i < n;) {
        while (i + j < n and S[j] == S[i + j]) j++;
        Z[i] = j;
        if (j == 0) {
            i++;
            continue;
        }
        int k = 1;
        while (k < j and k + Z[k] < j) {
            Z[i + k] = Z[k];
            k++;
        }
        i += k;
        j -= k;
    }
    return Z;
}
