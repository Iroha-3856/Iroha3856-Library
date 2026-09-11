//Z[i] = S と S[i, N) の最長共通接頭辞の長さ。O(N)
template<class T>
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
