# Alien DP / Lagrangian relaxation

個数 `k` を固定した最小化問題の値を `F(k)` とする。`F` が離散凸なら、制約を外して各要素へ罰金 `lambda` を加えた

`G(lambda) = min_k (F(k) + lambda * k)`

を解くことで `F` の傾きを探索できる。DP の各状態には `{罰金込みの値, 使用個数}` を持たせ、値が等しい場合は欲しい側の個数を選ぶ。

## 実装形

```cpp
//solve(lambda) は {min(F(k) + lambda*k), その中で最大の k}
template<class Solve>
ll alienDP(int wanted, ll low, ll high, const Solve& solve) {
    while (low < high) {
        ll mid = (low + high) / 2;
        if (solve(mid).second <= wanted) high = mid;
        else low = mid + 1;
    }
    auto [value, count] = solve(low);
    return value - low * wanted;
}
```

この形が正しいのは `F` の離散凸性が保証され、`wanted` が傾き `low` の劣勾配に含まれる場合である。単に「使用個数が単調に見える」だけでは、飛ばされた個数の値を復元できない。

Monge DAG の d 辺最短路では、辺数ごとの最短距離 `F(k)` が凸になることを示してから適用する。`solve(lambda)` は各辺重みに `lambda` を加えた通常の最短路であり、同距離なら辺数が多い方を採用する。最後に `lambda * d` を引く。
