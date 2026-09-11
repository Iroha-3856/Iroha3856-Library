//列の Cartesian Tree。O(N)、parent[root] = -1
//comp(a, b) が true の値を上へ置く。等値では左側の要素を上にする
template<class T, class Compare = less<T>>
vector<int> cartesianTree(const vector<T>& A, Compare comp = Compare()) {
    int N = (int)A.size();
    vector<int> parent(N, -1), st;
    for (int i = 0; i < N; i++) {
        int last = -1;
        while (!st.empty() and comp(A[i], A[st.back()])) {
            last = st.back();
            st.pop_back();
        }
        if (!st.empty()) parent[i] = st.back();
        if (last != -1) parent[last] = i;
        st.push_back(i);
    }
    return parent;
}
