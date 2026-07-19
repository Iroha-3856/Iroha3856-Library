#include <vector>
#include <limits>

template<class T>
struct MaxFlowGraph {
private:
    struct Edge {
        //to:行先
        int to;
        //cap:流量のあまり
        T cap;
        //逆辺のindex
        int rev;
    };
    //グラフ（隣接リスト）
    std::vector<std::vector<Edge> > G;
    //dfs用のすでに見たかを表す配列
    std::vector<bool> seen;
    //残余グラフ上でs-tパスを探し、流せる最大流量を返す
    T dfs(int v, int goal, T f) {
        //頂点tにたどり着いた
        if (v == goal) return f;
        seen[v] = true;
        for (int idx = 0; idx < (int)G[v].size(); idx++) {
            Edge& e = G[v][idx];
            if (seen[e.to]) continue;
            if (e.cap == 0) continue;
            T d = dfs(e.to, goal, std::min(f, e.cap));
            if (d > 0) {
                //s-tパスが見つかった
                //流量のあまりを減らす
                e.cap -= d;
                //逆辺の流量のあまりを増やす
                G[e.to][e.rev].cap += d;
                return d;
            }
        }
        return 0;
    }
public://コンストラクタ
    MaxFlowGraph() {
        //初期化
        G.resize(0);
        seen.resize(0);
    }
    //n頂点で初期化
    MaxFlowGraph(int n) {
        //初期化
        G.resize(n);
        seen.resize(n);
    }
    void add_edge(int u, int v, T cap) {
        G[u].push_back(Edge{v, cap, (int)G[v].size()});
        G[v].push_back(Edge{u, 0, (int)G[u].size()-1});
    }
    T max_flow(int s, int t) {
        T ret = 0;
        while(true) {
            seen.assign((int)seen.size(), false);
            T plus = dfs(s, t, std::numeric_limits<T>::max());
            if (plus == 0) break;
            else ret += plus;
        }
        return ret;
    }
};

#include <iostream>

int main() {
    int V, E;
    std::cin >> V >> E;
    MaxFlowGraph<int> G(V);
    for (int i = 0; i < E; i++) {
        int u, v; std::cin >> u >> v;
        int c; std::cin >> c;
        G.add_edge(u, v, c);
    }
    std::cout << G.max_flow(0, V-1) << std::endl;
}