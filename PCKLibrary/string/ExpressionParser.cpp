// 整数の数式を再帰下降法で評価する。時間 O(|s|)、空間 O(|s|)（括弧・単項符号の再帰）。
// 対応: 整数、空白、+, -, *, /、括弧 ()、単項 + / -。割り算は C++ と同じ 0 方向への切り捨て。
// 使いどころ: 通常の四則演算式を読み取り、演算子の優先順位と括弧を反映して値を求めたい場合。
// 使い方: ll answer = ExpressionParser("1 + 2 * (3 - 5)").evaluate(); // -3
// 入力は構文的に正しい式であり、0 除算と、途中結果の ll オーバーフローが起きないことを仮定する。
// 数値リテラルは 0 以上の整数で、単項 - による LLONG_MIN も受け付ける。
// 不正入力は assert で検出する（リリース版でのエラー処理 API は持たない）。
struct ExpressionParser {
    string s;
    int pos = 0;

    // 数式文字列を保持する。evaluate は複数回呼べる。
    explicit ExpressionParser(string expression) : s(std::move(expression)) {}

private:
    // 現在位置から空白を飛ばす。
    void skipSpaces() {
        while (pos < (int)s.size() and isspace((unsigned char)s[pos])) pos++;
    }
    // 次の文字が c なら読み進めて true を返す。
    bool consume(char c) {
        skipSpaces();
        if (pos == (int)s.size() or s[pos] != c) return false;
        pos++;
        return true;
    }
    // 中間結果が ll に収まることを確かめる。計算そのものは __int128_t で行う。
    __int128_t checked(__int128_t x) const {
        assert((__int128_t)LLONG_MIN <= x and x <= (__int128_t)LLONG_MAX);
        return x;
    }
    // 数字の列、または括弧に入った式を読み取る。
    __int128_t primary() {
        if (consume('(')) {
            __int128_t value = expression();
            bool closed = consume(')');
            assert(closed);
            (void)closed;
            return value;
        }
        skipSpaces();
        assert(pos < (int)s.size() and isdigit((unsigned char)s[pos]));
        __int128_t value = 0;
        const __int128_t limit = (__int128_t)LLONG_MAX + 1;
        while (pos < (int)s.size() and isdigit((unsigned char)s[pos])) {
            int digit = s[pos++] - '0';
            assert(value <= (limit - digit) / 10);
            value = value * 10 + digit;
        }
        return value;
    }
    // 単項 + と - を先に処理する。符号をまとめることで LLONG_MIN も扱える。
    __int128_t unary() {
        bool negative = false;
        while (true) {
            if (consume('+')) continue;
            if (consume('-')) negative = !negative;
            else break;
        }
        __int128_t value = primary();
        return checked(negative ? -value : value);
    }
    // * と / を左結合で処理する（同じ優先順位）。
    __int128_t term() {
        __int128_t value = unary();
        while (true) {
            if (consume('*')) value = checked(value * unary());
            else if (consume('/')) {
                __int128_t rhs = unary();
                assert(rhs != 0);
                value = checked(value / rhs);
            } else break;
        }
        return value;
    }
    // + と - を左結合で処理する。項 term より優先順位が低い。
    __int128_t expression() {
        __int128_t value = term();
        while (true) {
            if (consume('+')) value = checked(value + term());
            else if (consume('-')) value = checked(value - term());
            else break;
        }
        return value;
    }

public:
    // 式全体を評価して ll を返す。末尾に余計な文字がある場合は assert に失敗する。
    ll evaluate() {
        pos = 0;
        __int128_t value = expression();
        skipSpaces();
        assert(pos == (int)s.size());
        return (ll)value;
    }
};
