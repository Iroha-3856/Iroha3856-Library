// 列の Cartesian Tree。O(N)、parent[root] = -1
// comp(a, b) が true の値を上へ置く。等値では左側の要素を上にする
// 使いどころ: 配列の RMQ 構造や、各区間の最小値を根とする再帰分割を一つの木で表す場合。
// 使い方:
// vector<int> parent = cartesianTree(A); で最小 Cartesian Tree を得る。
// parent[v] が親で、-1 の頂点が根。子の左右は元添字との大小から判定する。
// 最大木は cartesianTree(A, greater<T>())。RMQ や部分配列最小値の木構造化に使う。
// Cartesian Tree は inorder 順が元配列の添字順で、親の値が子より良い heap になる二分木。
// 具体例: A={3, 1, 2} の最小木では添字1が根、添字0と2がその左右の子になる。
// 返すのは親配列だけで、左子・右子は child<parent または child>parent から区別する。
template<class T, class Compare = less<T>>
// A の添字順を inorder、comp で最良の値を根とする Cartesian Tree の親配列を返す。
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
