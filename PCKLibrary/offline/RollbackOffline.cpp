//rollback Mo。snapshot() の返す状態へ rollback(state) で戻せること
//add(i) が O(T) なら O((N + Q)sqrt(N)T)、区間は [l, r)
struct RollbackMo {
    struct Query { int l, r, id; };
    int N;
    vector<Query> query;

    RollbackMo(int n) : N(n) {}
    int addQuery(int l, int r) {
        int id = (int)query.size();
        query.push_back({l, r, id});
        return id;
    }
    template<class Snapshot, class Rollback, class Add, class Answer>
    void run(const Snapshot& snapshot, const Rollback& rollback,
             const Add& add, const Answer& answer) const {
        int block = max(1, (int)sqrt(max(1, N)));
        vector<vector<Query>> bucket((N + block - 1) / block);
        for (Query q : query) bucket[q.l / block].push_back(q);
        for (int blockId = 0; blockId < (int)bucket.size(); blockId++) {
            int border = min(N, (blockId + 1) * block);
            vector<Query> small, large;
            for (Query q : bucket[blockId]) {
                if (q.r <= border) small.push_back(q);
                else large.push_back(q);
            }
            for (Query q : small) {
                auto state = snapshot();
                for (int i = q.l; i < q.r; i++) add(i);
                answer(q.id);
                rollback(state);
            }
            sort(large.begin(), large.end(), [](Query a, Query b) { return a.r < b.r; });
            auto base = snapshot();
            int r = border;
            for (Query q : large) {
                while (r < q.r) add(r++);
                auto state = snapshot();
                for (int i = border - 1; i >= q.l; i--) add(i);
                answer(q.id);
                rollback(state);
            }
            rollback(base);
        }
    }
};

//時間軸 Segment Tree。区間 [l, r) で有効な operation を追加する
//run は節点進入時 apply、退出時 undo、各時刻で answer(time) を呼ぶ
template<class Operation>
struct DivideAndConquerOnTime {
    int Q, siz;
    vector<vector<Operation>> data;

    DivideAndConquerOnTime(int q) : Q(q) {
        siz = 1;
        while (siz < Q) siz <<= 1;
        data.resize(2 * siz);
    }
    void addInterval(int l, int r, const Operation& operation) {
        assert(0 <= l and l <= r and r <= Q);
        l += siz; r += siz;
        while (l < r) {
            if (l & 1) data[l++].push_back(operation);
            if (r & 1) data[--r].push_back(operation);
            l >>= 1; r >>= 1;
        }
    }
    template<class Apply, class Undo, class Answer>
    void dfs(int k, const Apply& apply, const Undo& undo, const Answer& answer) const {
        for (const Operation& x : data[k]) apply(x);
        if (k < siz) {
            dfs(2 * k, apply, undo, answer);
            dfs(2 * k + 1, apply, undo, answer);
        } else if (k - siz < Q) {
            answer(k - siz);
        }
        for (int i = (int)data[k].size() - 1; i >= 0; i--) undo(data[k][i]);
    }
    template<class Apply, class Undo, class Answer>
    void run(const Apply& apply, const Undo& undo, const Answer& answer) const {
        dfs(1, apply, undo, answer);
    }
};
