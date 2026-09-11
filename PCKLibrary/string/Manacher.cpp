//Manacher 法。odd[i] は中心 i の奇数半径、even[i] は中心 i-1,i の偶数半径
//回文長はそれぞれ 2*odd[i]-1, 2*even[i]。O(N)
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
