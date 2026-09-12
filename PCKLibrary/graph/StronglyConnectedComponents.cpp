// 有向グラフの強連結成分分解。O(N + M)
// component[v] は縮約 DAG のトポロジカル順になる
// 使い方:
// StronglyConnectedComponents scc(G); と構築するだけで分解が完了する。
// scc.component[v] が成分番号、scc.groups[id] が所属頂点、scc.dag が重複辺なしの縮約 DAG。
// component の番号は dag のトポロジカル順なので、辺は小さい番号から大きい番号へ向かう。
// 使いどころ: 有向閉路を一頂点へまとめる、2-SAT、相互到達可能性の判定。
// 具体例: 0->1, 1->0, 1->2 なら groups は {0, 1} と {2} の二成分になる。
// groups 内の頂点順には意味を持たせず、成分間の処理順には component 番号を使う。
struct StronglyConnectedComponents {
    int N;
    vector<vector<int>> G, rG, groups, dag;
    vector<int> component;

    // 有向隣接リスト g を受け取り、SCC と縮約 DAG まで直ちに構築する。
    StronglyConnectedComponents(const vector<vector<int>>& g) : N((int)g.size()), G(g), rG(N) {
        for (int v = 0; v < N; v++) for (int to : G[v]) rG[to].push_back(v);
        build();
    }
    // Kosaraju 法で groups/component を求め、重複辺なしの dag を構築する。
    void build() {
        vector<int> used(N), order;
        for (int s = 0; s < N; s++) if (!used[s]) {
            vector<pair<int, int>> st = {{s, 0}};
            used[s] = true;
            while (!st.empty()) {
                int v = st.back().first;
                int& i = st.back().second;
                if (i < (int)G[v].size()) {
                    int to = G[v][i++];
                    if (!used[to]) used[to] = true, st.push_back({to, 0});
                } else {
                    order.push_back(v);
                    st.pop_back();
                }
            }
        }
        component.assign(N, -1);
        reverse(order.begin(), order.end());
        for (int s : order) if (component[s] == -1) {
            int id = (int)groups.size();
            groups.push_back({});
            vector<int> st = {s};
            component[s] = id;
            while (!st.empty()) {
                int v = st.back(); st.pop_back();
                groups[id].push_back(v);
                for (int to : rG[v]) if (component[to] == -1) {
                    component[to] = id;
                    st.push_back(to);
                }
            }
        }
        dag.resize(groups.size());
        for (int v = 0; v < N; v++) for (int to : G[v]) {
            int a = component[v], b = component[to];
            if (a != b) dag[a].push_back(b);
        }
        for (auto& A : dag) {
            sort(A.begin(), A.end());
            A.erase(unique(A.begin(), A.end()), A.end());
        }
    }
};
