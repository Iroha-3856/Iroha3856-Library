// 半開区間 [l, r) の和集合を、重複・隣接しない極大区間にまとめて管理する。
// 区間数を K、更新で消える区間数を t とすると insert/erase は O(log K + t)、検索は O(log K)。
// 使いどころ: 塗られた整数区間の追加・削除、区間の包含判定、mex や被覆長の管理。
// 使い方:
// RangeSet rs; rs.insert(2, 5); rs.insert(5, 8); // intervals() は {[2,8)}
// rs.erase(4, 6); // {[2,4), [6,8)}。contains(3) は true、mex(2) は 4。
// 空区間の追加・削除は何もしない。contains(l, l) は true とする。
// 端点は ll、被覆長は全 ll 範囲でもオーバーフローしない __int128_t を返す。
// intervals() の返す map のキーが左端、値が右端。直接変更しないこと。
struct RangeSet {
private:
    map<ll, ll> seg;
    __int128_t length = 0;

public:
    // 全区間の個数を返す。隣接区間は必ず統合済み。
    int size() const { return (int)seg.size(); }
    // 何も被覆していなければ true を返す。
    bool empty() const { return seg.empty(); }
    // 保持する極大区間 [左端, 右端) を左端順で参照する。
    const map<ll, ll>& intervals() const { return seg; }
    // 被覆されている整数点の総数を返す。結果は __int128_t。
    __int128_t coveredLength() const { return length; }

    // [l, r) を追加し、重複または隣接する区間をまとめる。l <= r が必要。
    void insert(ll l, ll r) {
        assert(l <= r);
        if (l == r) return;
        auto it = seg.lower_bound(l);
        if (it != seg.begin() and prev(it)->second >= l) --it;
        while (it != seg.end() and it->first <= r) {
            l = min(l, it->first);
            r = max(r, it->second);
            length -= (__int128_t)it->second - it->first;
            it = seg.erase(it);
        }
        seg.emplace_hint(it, l, r);
        length += (__int128_t)r - l;
    }

    // [l, r) を削除する。区間の途中を削除した場合は左右へ分割する。
    void erase(ll l, ll r) {
        assert(l <= r);
        if (l == r) return;
        auto it = seg.lower_bound(l);
        if (it != seg.begin()) --it;
        while (it != seg.end() and it->first < r) {
            ll a = it->first, b = it->second;
            if (b <= l) {
                ++it;
                continue;
            }
            it = seg.erase(it);
            length -= (__int128_t)b - a;
            if (a < l) {
                seg.emplace_hint(it, a, l);
                length += (__int128_t)l - a;
            }
            if (r < b) {
                seg.emplace_hint(it, r, b);
                length += (__int128_t)b - r;
                break;
            }
        }
    }

    // 整数点 x が被覆されていれば true を返す。
    bool contains(ll x) const {
        auto it = seg.upper_bound(x);
        return it != seg.begin() and x < prev(it)->second;
    }
    // [l, r) が全て被覆されていれば true。空区間は true。
    bool contains(ll l, ll r) const {
        assert(l <= r);
        if (l == r) return true;
        auto it = seg.upper_bound(l);
        return it != seg.begin() and prev(it)->second >= r;
    }
    // x 以上で最初の未被覆整数を返す。区間端点として LLONG_MAX も返り得る。
    // x == LLONG_MAX のときも LLONG_MAX を返す（それより大きい値は表現しない）。
    ll mex(ll x) const {
        auto it = seg.upper_bound(x);
        if (it != seg.begin() and x < prev(it)->second) return prev(it)->second;
        return x;
    }
};
