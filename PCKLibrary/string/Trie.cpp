//文字が [Margin, Margin + Sigma) に含まれる固定アルファベットの Trie。各操作 O(|S|)
template<int Sigma = 26, char Margin = 'a'>
struct Trie {
    vector<array<int, Sigma>> next;
    vector<int> terminal, pass;

    Trie() {
        addNode();
    }
    int addNode() {
        array<int, Sigma> A;
        A.fill(-1);
        next.push_back(A);
        terminal.push_back(0);
        pass.push_back(0);
        return (int)next.size() - 1;
    }
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
    int find(const string& S) const {
        int v = 0;
        for (char c : S) {
            int x = c - Margin;
            if (next[v][x] == -1) return -1;
            v = next[v][x];
        }
        return v;
    }
    int count(const string& S) const {
        int v = find(S);
        return v == -1 ? 0 : terminal[v];
    }
};
