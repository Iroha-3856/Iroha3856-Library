// 文字が [Margin, Margin + Sigma) に含まれる固定アルファベットの Trie。各操作 O(|S|)
// 使いどころ: 登録文字列の完全一致個数や接頭辞個数を、文字列長に比例する時間で更新・取得する場合。
// 具体例: "app", "apple", "app" を insert すると count("app")=2、節点 "app" の pass は 3。
// 使い方:
// Trie<> trie; int node = trie.insert(s); で文字列を一つ追加する。重複追加も個数として保持する。
// find(s) は対応節点、存在しなければ -1。count(s) は完全一致で登録された個数。
// 接頭辞 s を持つ登録文字列数は int v=trie.find(s); の後、v==-1 ? 0 : trie.pass[v]。
// 文字種を変える場合は Trie<Sigma, Margin> とし、入力文字が範囲内であることを保証する。
template<int Sigma = 26, char Margin = 'a'>
struct Trie {
    vector<array<int, Sigma>> next;
    vector<int> terminal, pass;

    // 根節点だけを持つ空 Trie を構築する。
    Trie() {
        addNode();
    }
    // 遷移なし・個数 0 の節点を追加し、その節点番号を返す。
    int addNode() {
        array<int, Sigma> A;
        A.fill(-1);
        next.push_back(A);
        terminal.push_back(0);
        pass.push_back(0);
        return (int)next.size() - 1;
    }
    // 文字列 S を一つ登録し、その終端節点番号を返す。
    int insert(const string& S) {
        int v = 0;
        pass[v]++;
        for (char c : S) {
            int x = c - Margin;
            if (next[v][x] == -1) next[v][x] = addNode();
            v = next[v][x];
            pass[v]++;
        }
        terminal[v]++;
        return v;
    }
    // S に対応する節点番号を返す。経路がなければ -1。
    int find(const string& S) const {
        int v = 0;
        for (char c : S) {
            int x = c - Margin;
            if (next[v][x] == -1) return -1;
            v = next[v][x];
        }
        return v;
    }
    // 文字列 S が完全一致で登録された個数を返す。
    int count(const string& S) const {
        int v = find(S);
        return v == -1 ? 0 : terminal[v];
    }
};
