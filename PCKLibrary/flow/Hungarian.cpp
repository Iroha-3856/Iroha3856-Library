// N <= M の長方形コスト行列の最小重み割当。O(N^2 M)
// {最小コスト, assignment}、assignment[i] は行 i に割り当てた列
// 使い方:
// auto [value, assignment] = hungarian(cost); とし、各行へ異なる列を一つずつ割り当てる。
// cost は N 行 M 列で N<=M。assignment[i] が行 i に選ばれた列、value が総コスト。
// 負コストも扱える。最大化したい場合は全コストの符号を反転して呼ぶ。
// 使いどころ: 各作業へ異なる担当者を一人ずつ割り当て、総コストを最小化する場合。
// 具体例: cost={{4, 1}, {2, 3}} なら行0->列1、行1->列0で最小値3。
// 行はすべて割り当てるが、列は M-N 個余ってよい。割当不能を表すなら十分大きいコストを置く。
// cost[i][j] を払って各行 i に異なる列 j を割り当てる最小総費用と割当を返す。
pair<ll, vector<int>> hungarian(const vector<vector<ll>>& cost) {
    int N = (int)cost.size();
    if (N == 0) return {0, {}};
    int M = (int)cost[0].size();
    assert(N <= M);
    vector<ll> u(N + 1), v(M + 1);
    vector<int> match(M + 1), way(M + 1);
    for (int i = 1; i <= N; i++) {
        match[0] = i;
        int j0 = 0;
        vector<ll> minCost(M + 1, INF);
        vector<bool> used(M + 1);
        do {
            used[j0] = true;
            int i0 = match[j0], j1 = 0;
            ll delta = INF;
            for (int j = 1; j <= M; j++) if (!used[j]) {
                ll cur = cost[i0 - 1][j - 1] - u[i0] - v[j];
                if (chmin(minCost[j], cur)) way[j] = j0;
                if (minCost[j] < delta) delta = minCost[j], j1 = j;
            }
            for (int j = 0; j <= M; j++) {
                if (used[j]) u[match[j]] += delta, v[j] -= delta;
                else minCost[j] -= delta;
            }
            j0 = j1;
        } while (match[j0] != 0);
        do {
            int j1 = way[j0];
            match[j0] = match[j1];
            j0 = j1;
        } while (j0 != 0);
    }
    vector<int> assignment(N, -1);
    for (int j = 1; j <= M; j++) if (match[j]) assignment[match[j] - 1] = j - 1;
    return {-v[0], assignment};
}
