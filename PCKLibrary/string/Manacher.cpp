// Manacher 法。odd[i] は中心 i の奇数半径、even[i] は中心 i-1, i の偶数半径
// 回文長はそれぞれ 2*odd[i]-1, 2*even[i]。O(N)
// 使いどころ: 全中心の最長回文や、任意区間が回文かを線形前計算後に判定する場合。
// 具体例: s="abba" では even[2]=2 なので、中心境界 1|2 の回文区間は [0, 4)。
// 使い方:
// auto [odd, even] = manacher(s); とする。
// 中心 i の最大奇数回文は [i-odd[i]+1, i+odd[i])。
// 中心境界 i-1|i の最大偶数回文は [i-even[i], i+even[i])。
// ある区間が回文かは、その中心の半径が必要長以上かで O(1) 判定できる。
// 各文字中心の奇数回文半径と、各文字間中心の偶数回文半径を返す。
pair<vector<int>, vector<int>> manacher(const string& s) {
    int N = (int)s.size();
    vector<int> odd(N), even(N);
    for (int i = 0, l = 0, r = -1; i < N; i++) {
        int k = i > r ? 1 : min(odd[l + r - i], r - i + 1);
        while (0 <= i - k and i + k < N and s[i - k] == s[i + k]) k++;
        odd[i] = k--;
        if (r < i + k) l = i - k, r = i + k;
    }
    for (int i = 0, l = 0, r = -1; i < N; i++) {
        int k = i > r ? 0 : min(even[l + r - i + 1], r - i + 1);
        while (0 <= i - k - 1 and i + k < N and s[i - k - 1] == s[i + k]) k++;
        even[i] = k--;
        if (r < i + k) l = i - k - 1, r = i + k;
    }
    return {odd, even};
}
