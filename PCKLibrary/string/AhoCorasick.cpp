//小文字列用 Aho-Corasick。構築 O(総パターン長 * SIGMA)、検索 O(|text| + 出現数集計)
//countMatches は各 addPattern のパターンが text に現れる回数を返す
template<int SIGMA = 26, char BASE = 'a'>
struct AhoCorasick {
    struct Node {
        array<int, SIGMA> next;
        int fail = 0;
        vector<int> pattern;
        Node() { next.fill(-1); }
    };
    vector<Node> node = vector<Node>(1);
    vector<int> bfsOrder;
    int patterns = 0;

    int addPattern(const string& s) {
        int v = 0;
        for (char c : s) {
            int x = c - BASE;
            assert(0 <= x and x < SIGMA);
            if (node[v].next[x] == -1) {
                node[v].next[x] = (int)node.size();
                node.push_back(Node());
            }
            v = node[v].next[x];
        }
        node[v].pattern.push_back(patterns);
        return patterns++;
    }
    void build() {
        queue<int> Q;
        bfsOrder = {0};
        for (int c = 0; c < SIGMA; c++) {
            int to = node[0].next[c];
            if (to == -1) node[0].next[c] = 0;
            else node[to].fail = 0, Q.push(to);
        }
        while (!Q.empty()) {
            int v = Q.front(); Q.pop();
            bfsOrder.push_back(v);
            for (int c = 0; c < SIGMA; c++) {
                int to = node[v].next[c];
                if (to == -1) node[v].next[c] = node[node[v].fail].next[c];
                else {
                    node[to].fail = node[node[v].fail].next[c];
                    Q.push(to);
                }
            }
        }
    }
    vector<ll> countMatches(const string& s) const {
        vector<ll> visit(node.size()), ret(patterns);
        int v = 0;
        for (char c : s) {
            int x = c - BASE;
            assert(0 <= x and x < SIGMA);
            v = node[v].next[x];
            visit[v]++;
        }
        for (int i = (int)bfsOrder.size() - 1; i; i--) {
            int x = bfsOrder[i];
            visit[node[x].fail] += visit[x];
        }
        for (int x = 0; x < (int)node.size(); x++) {
            for (int id : node[x].pattern) ret[id] = visit[x];
        }
        return ret;
    }
};
