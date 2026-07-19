//ソートされた状態で約数を得る
template<class T> 
vector<T> divisors(T n) {
    vector<T> ret_small, ret_big;
    for (T d = 1; d * d <= n; d++) {
        if (n % d) continue;
        ret_small.push_back(d);
        if (n != d * d) ret_big.push_back(n / d);
    }
    reverse(all(ret_big));
    ret_small.insert(ret_small.end(), all(ret_big));
    return ret_small;
}