//RollBack可能Union-Find
struct RollbackUnionFind {
    int sz;
    vector<int> data;
    stack<pair<int, int>> history;
    int last;
    int connected_components;
    
    RollbackUnionFind() : RollbackUnionFind(0) {}
    RollbackUnionFind(int n) {
        sz = n;
        data.assign(n, -1);
        last = 0;
        connected_components = n;
    }
    bool merge(int x, int y) {
        x = find(x), y = find(y);
        history.emplace(x, data[x]);
        history.emplace(y, data[y]);
        //size(x) > size(y)であることを保証する
        if (data[x] > data[y]) swap(x, y);
        if (x == y) {
            return false;
        }
        connected_components--;
        data[x] += data[y];
        data[y] = x;
        return true;
    }
    int find(int x) {
        if (data[x] < 0) return x;
        return find(data[x]);
    }
    bool same(int x, int y) { return find(x) == find(y); }
    int size(int x) { return -data[find(x)]; }

    void undo() {
        auto[x, datax] = history.top(); history.pop();
        data[x] = datax;
        auto[y, datay] = history.top(); history.pop();
        data[y] = datay;
        if (x != y) connected_components++;
    }
    void snapshot() { last = ((int)history.size()>>1); }

    int getState() { return (int)history.size()>>1; }

    void rollback(int state = -1) {
        if (state == -1) state = last;
        state <<= 1;
        assert(state <= (int)history.size());
        while((int)history.size() > state) undo();
    }

    int connectedComponents() {
        return connected_components;
    }
    int size() {
        return sz;
    }
};