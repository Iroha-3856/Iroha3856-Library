// 直線 y = ax + b の最小値。傾きは狭義単調減少で追加する
// addLine 償却 O(1)、x 単調増加 query O(1)、任意順 query O(log N)
// 使いどころ: DP 遷移を「過去の状態が作る直線」と「現在の x での最小値」に変形できる場合。
// 具体例: y=2x+3, y=-x+5 の順に追加すると query(2)=3。後者の値 -2+5 を選ぶ。
// 使い方:
// ConvexHullTrick cht; cht.addLine(a, b); を a の狭義降順で行う。
// x も非減少なら queryMonotone(x)、任意順なら query(x) で min(ax+b) を得る。
// queryMonotone は内部 head を進める破壊的操作なので、その後に小さい x を問い合わせない。
// 同傾きの直線は事前に切り詰め、a*x+b が ll に収まること。
struct ConvexHullTrick {
    struct Line {
        ll a, b;
        // 座標 x における直線 ax+b の値を返す。
        ll get(ll x) const { return a * x + b; }
    };
    int head = 0;
    vector<Line> line;

    // 傾き順の三直線 a, b, c のうち、最小値包絡で b が不要かを 128 bit 内で厳密判定する。
    bool unnecessary(Line a, Line b, Line c) const {
        __int128_t x = (__int128_t)b.b - a.b;
        __int128_t y = (__int128_t)c.b - b.b;
        __uint128_t p = (__uint128_t)((__int128_t)b.a - c.a);
        __uint128_t q = (__uint128_t)((__int128_t)a.a - b.a);
        if (x >= 0 and y < 0) return true;
        if (x < 0 and y >= 0) return false;
        __uint128_t ax = x < 0 ? (__uint128_t)(-x) : (__uint128_t)x;
        __uint128_t ay = y < 0 ? (__uint128_t)(-y) : (__uint128_t)y;
        if (x >= 0) return ax * p >= ay * q;
        return ax * p <= ay * q;
    }
    // 傾き a が既存末尾より真に小さい直線 y=ax+b を追加する。
    void addLine(ll a, ll b) {
        assert(line.empty() or line.back().a > a);
        Line x{a, b};
        while ((int)line.size() - head >= 2
               and unnecessary(line[line.size() - 2], line.back(), x)) line.pop_back();
        line.push_back(x);
    }
    // 非減少な x に対する最小値を返し、不要になった先頭を head で読み飛ばす。
    ll queryMonotone(ll x) {
        assert(!line.empty());
        while (head + 1 < (int)line.size() and line[head + 1].get(x) <= line[head].get(x)) head++;
        return line[head].get(x);
    }
    // 任意順の x に対する最小値を三分探索相当の二分探索で返す。
    ll query(ll x) const {
        assert(!line.empty());
        int l = head, r = (int)line.size() - 1;
        while (l < r) {
            int m = (l + r) / 2;
            if (line[m + 1].get(x) <= line[m].get(x)) l = m + 1;
            else r = m;
        }
        return line[l].get(x);
    }
};
