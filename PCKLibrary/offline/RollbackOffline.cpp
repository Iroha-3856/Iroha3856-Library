// rollback Mo。snapshot() の返す状態へ rollback(state) で戻せること
// add(i) が O(T) なら O((N + Q)sqrt(N)T)、区間は [l, r)
// 使いどころ: 削除は難しいが、履歴を巻き戻せる DSU などで区間・時刻 query をまとめて処理する場合。
// 具体例: 辺 e が時刻2から5だけ存在するなら addInterval(2, 5, e) とし、各葉で連結性を答える。
// 使い方:
// RollbackMo mo(N); へ query を登録し mo.run(snapshot, rollback, add, answer) を呼ぶ。
// snapshot() は履歴長等の軽い印、rollback(state) はその状態まで全 add を巻き戻す。
// DivideAndConquerOnTime<Op> timeline(Q) は operation の有効期間 [l, r) を addInterval で登録する。
// timeline.run(apply, undo, answer) は各時刻で有効な操作だけ適用し answer(time) を呼ぶ。
// undo(op) は apply(op) の厳密な逆で、同一節点内では追加と逆順に呼ばれる。
struct RollbackMo {
    struct Query { int l, r, id; };
    int N;
    vector<Query> query;

    // 添字範囲 [0, n) を持つ空の rollback Mo query 集合を作る。
    RollbackMo(int n) : N(n) {}
    // 半開区間 [l, r) を追加し、answer に渡される query ID を返す。
    int addQuery(int l, int r) {
        int id = (int)query.size();
        query.push_back({l, r, id});
        return id;
    }
    // 右端を単調に伸ばし、左端の一時追加を rollback しながら全 query を処理する。
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
            // 右端だけを追加していけるよう、同じ左 block の長い query を r 昇順に並べる。
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

// 時間軸 Segment Tree。区間 [l, r) で有効な operation を追加する
// run は節点進入時 apply、退出時 undo、各時刻で answer(time) を呼ぶ
template<class Operation>
struct DivideAndConquerOnTime {
    int Q, siz;
    vector<vector<Operation>> data;

    // 時刻 0, ..., q-1 を葉に持つ空の時間軸 Segment Tree を作る。
    DivideAndConquerOnTime(int q) : Q(q) {
        siz = 1;
        while (siz < Q) siz <<= 1;
        data.resize(2 * siz);
    }
    // operation が有効な半開時間区間 [l, r) を Segment Tree の節点へ分解して登録する。
    void addInterval(int l, int r, const Operation& operation) {
        assert(0 <= l and l <= r and r <= Q);
        l += siz; r += siz;
        while (l < r) {
            if (l & 1) data[l++].push_back(operation);
            if (r & 1) data[--r].push_back(operation);
            l >>= 1; r >>= 1;
        }
    }
    // 節点進入時に操作を適用し、葉で answer(time)、退出時に逆順で undo する。
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
    // 時刻 0 から Q-1 までの answer を DFS 順に一度ずつ呼ぶ。
    template<class Apply, class Undo, class Answer>
    void run(const Apply& apply, const Undo& undo, const Answer& answer) const {
        dfs(1, apply, undo, answer);
    }
};
