// 小文字列用 Aho-Corasick。構築 O(総パターン長 * SIGMA)、検索 O(|text| + 出現数集計)
// countMatches は各 addPattern のパターンが text に現れる回数を返す
// 使いどころ: 一つの長い text に対し、多数の pattern の出現回数を同時に数える場合。
// 具体例: "a", "aa" を登録して text="aaa" を検索すると、各出現回数は 3, 2。
// 使い方:
// AhoCorasick<> ac; int id = ac.addPattern(pattern); として全パターンを登録してから ac.build();
// vector<ll> count = ac.countMatches(text); の count[id] が重なりを含む出現回数。空 pattern は |text|+1 回。
// build 後は遷移表が完成形へ変わるためパターンを追加しない。文字は [BASE, BASE+SIGMA) に限る。
// 英大文字などは AhoCorasick<26, 'A'>、二進文字列なら AhoCorasick<2, '0'> とする。
template<int SIGMA = 26, char BASE = 'a'>
struct AhoCorasick {
    struct Node {
        array<int, SIGMA> next;
        int fail = 0;
        vector<int> pattern;
        // 全文字遷移を未作成 -1 とする Trie 節点を初期化する。
        Node() { next.fill(-1); }
    };
    vector<Node> node = vector<Node>(1);
    vector<int> bfsOrder;
    int patterns = 0;

    // パターン s を Trie へ追加し、countMatches の返却添字となる ID を返す。
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
    // failure link と不足遷移を BFS で構築する。全パターン追加後に一度呼ぶ。
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
    // text 内で各登録パターンが現れる回数を ID 順で返す。build 後に呼ぶ。
    vector<ll> countMatches(const string& s) const {
        vector<ll> visit(node.size()), ret(patterns);
        visit[0] = 1;
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
