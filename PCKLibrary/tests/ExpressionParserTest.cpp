// g++ -std=gnu++17 -O2 -Wall -Wextra -Werror PCKLibrary/tests/ExpressionParserTest.cpp -o /tmp/expression_parser_test && /tmp/expression_parser_test
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#include "../string/ExpressionParser.cpp"

int main() {
    auto check = [](const string& s, ll expected) {
        ExpressionParser parser(s);
        assert(parser.evaluate() == expected);
        assert(parser.evaluate() == expected);
    };
    // 演算子の優先順位・結合性・負数除算・単項符号・括弧・空白。
    vector<pair<string, ll>> cases = {
        {"0", 0}, {"   \t 00000123 \n", 123},
        {"1 + 2 * (3 - 5)", -3}, {"(1 + 2) * (3 - 5)", -6},
        {"8 - 3 - 2", 3}, {"20 / 3 / 2", 3},
        {"8 / -3", -2}, {"-8 / 3", -2}, {"-8 / -3", 2},
        {"7 / -2", -3}, {"1--2", 3}, {"1 - - 2", 3},
        {"2*-3", -6}, {"-  2 * -3", 6}, {"-+(+2 + -3) * -4", -4},
        {"-(1 + 3)", -4}, {"9 + 12 / 3 * 2", 17},
        {"0 / (2 - 1)", 0}, {"-0", 0}, {"+ + - - 14", 14},
        {"-(-(-7))", -7},
        {"9223372036854775807", LLONG_MAX},
        {"-9223372036854775808", LLONG_MIN},
        {"(-9223372036854775808) / 2", LLONG_MIN / 2},
        {"9223372036854775807 - 9223372036854775807", 0},
        {"(-9223372036854775807 - 1) / 2", LLONG_MIN / 2},
        {"-((123456789 * 23456789) / 3)", -(123456789LL * 23456789 / 3)},
        {"(2 + 3) * (4 + 5) / (6 - 1)", 9}
    };
    for (const auto& [s, value] : cases) check(s, value);
    check(string(1000, '(') + "7" + string(1000, ')'), 7);

    mt19937_64 rng(20261107);
    struct Generated { string text; ll value; };
    // AST に直接演算を適用して正解を作り、完全括弧付きの式へ変換する。
    function<Generated(int)> generate = [&](int depth) -> Generated {
        if (depth == 0 or rng() % 4 == 0) {
            ll x = (ll)(rng() % 201) - 100;
            return {to_string(x), x};
        }
        if (rng() % 5 == 0) {
            auto x = generate(depth - 1);
            if (rng() % 2) return {"(-(" + x.text + "))", -x.value};
            return {"(+(" + x.text + "))", x.value};
        }
        auto a = generate(depth - 1), b = generate(depth - 1);
        char op = "+-*/"[rng() % 4];
        if (op == '/' and b.value == 0) return generate(depth - 1);
        __int128_t value = 0;
        if (op == '+') value = (__int128_t)a.value + b.value;
        if (op == '-') value = (__int128_t)a.value - b.value;
        if (op == '*') value = (__int128_t)a.value * b.value;
        if (op == '/') value = a.value / b.value;
        if (value < -1000000000000LL or value > 1000000000000LL) return generate(depth - 1);
        string text = "(" + a.text + " ";
        text += op;
        text += " " + b.text + ")";
        return {text, (ll)value};
    };
    const int AST_CASES = 40000;
    for (int i = 0; i < AST_CASES; i++) {
        auto x = generate(6);
        check(x.text, x.value);
    }

    // 括弧のない式では各 * / 列を先に計算し、その結果を + - で結合する。
    const int FLAT_CASES = 20000;
    for (int i = 0; i < FLAT_CASES; i++) {
        string s;
        ll expected = 0;
        int terms = 1 + (int)(rng() % 8);
        for (int t = 0; t < terms; t++) {
            int factors = 1 + (int)(rng() % 5);
            ll value = 0;
            string part;
            for (int j = 0; j < factors; j++) {
                ll x = (ll)(rng() % 19) - 9;
                if (j > 0 and x == 0) x = 1;
                string atom = to_string(x);
                if (rng() % 3 == 0) atom = "+" + atom;
                if (rng() % 6 == 0) atom = "-" + atom, x = -x;
                if (j == 0) value = x, part = atom;
                else {
                    char op = (rng() % 2) ? '*' : '/';
                    if (op == '*') value *= x;
                    else value /= x;
                    part += " "; part += op; part += " " + atom;
                }
            }
            if (t == 0) expected = value, s = part;
            else if (rng() % 2) expected += value, s += " + " + part;
            else expected -= value, s += " - " + part;
        }
        check(s, expected);
    }
    cout << "ExpressionParser tests passed: " << AST_CASES + FLAT_CASES
         << " randomized expressions\n";
}
