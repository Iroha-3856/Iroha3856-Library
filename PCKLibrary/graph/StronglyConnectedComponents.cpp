//有向グラフの強連結成分分解。O(N + M)
//component[v] は縮約 DAG のトポロジカル順になる
struct StronglyConnectedComponents {
    int N;
    vector<vector<int>> G, rG, groups, dag;
    vector<int> component;

    StronglyConnectedComponents(const vector<vector<int>>& g) : N((int)g.size()), G(g), rG(N) {
        for (int v = 0; v < N; v++) for (int to : G[v]) rG[to].push_back(v);
        build();
    }
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
