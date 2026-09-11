//直線 y = ax + b の最小値。傾きは狭義単調減少で追加する
//addLine 償却 O(1)、x 単調増加 query O(1)、任意順 query O(log N)
struct ConvexHullTrick {
    struct Line {
        ll a, b;
        ll get(ll x) const { return a * x + b; }
    };
    vector<Line> line;
    int head = 0;

    bool unnecessary(Line a, Line b, Line c) const {
        return (__int128_t)(b.b - a.b) * (b.a - c.a)
            >= (__int128_t)(c.b - b.b) * (a.a - b.a);
    }
    void addLine(ll a, ll b) {
        assert(line.empty() or line.back().a > a);
        Line x{a, b};
        while ((int)line.size() - head >= 2
               and unnecessary(line[line.size() - 2], line.back(), x)) line.pop_back();
        line.push_back(x);
    }
    ll queryMonotone(ll x) {
        assert(!line.empty());
        while (head + 1 < (int)line.size() and line[head + 1].get(x) <= line[head].get(x)) head++;
        return line[head].get(x);
    }
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
