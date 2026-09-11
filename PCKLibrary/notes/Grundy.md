# Grundy 数

## 基本

有限 DAG 上の impartial game では、状態 `v` の Grundy 数を次で定める。

```cpp
grundy[v] = mex({grundy[to] | v から to へ遷移可能})
```

`mex(S)` は `S` に含まれない最小の非負整数。遷移先がない状態の Grundy 数は `0` になる。

- Grundy 数が `0` なら後手必勝、`0` でなければ先手必勝。
- 独立なゲームを同時に遊ぶ直和の Grundy 数は、それぞれの Grundy 数の XOR。
- 勝敗だけなら `win[v] = any(!win[to])` で足りる。ゲームの合成があるときに Grundy 数が必要。

## DAG 上の実装

```cpp
vector<int> grundy(N, -1);

auto dfs = [&](auto dfs, int v) -> int {
    if (grundy[v] != -1) return grundy[v];
    vector<int> next;
    for (int to : G[v]) next.push_back(dfs(dfs, to));
    sort(next.begin(), next.end());
    next.erase(unique(next.begin(), next.end()), next.end());
    int ret = 0;
    for (int x : next) {
        if (x == ret) ret++;
        else if (x > ret) break;
    }
    return grundy[v] = ret;
};
```

遷移数が `D` 以下なら Grundy 数も `D` 以下なので、毎回 `set` を作らず時刻配列で mex を求めてもよい。

```cpp
vector<int> used(D + 1, -1);
for (int turn = 0; turn < Q; turn++) {
    for (int x : nextGrundy) if (x <= D) used[x] = turn;
    int mex = 0;
    while (used[mex] == turn) mex++;
}
```

## 状態が大きい場合

- 山の大きさなどが整数一つで表されるなら、小さい範囲を計算して周期を疑う。
- 周期を観察しただけでは証明にならない。遷移が参照する範囲と mex が一致し続けることを示す。
- 状態を正規化できる場合は、同型な状態を同じキーにしてメモ化する。
- ループを含むゲームには通常の Grundy 数の DAG DP をそのまま適用できない。

## 典型チェック

1. 両者の可能な操作が同じ impartial game か。
2. 必ず有限手で終了するか。
3. 複数部分に分解した後、それぞれが独立か。
4. 最後に動けない側が負ける normal play か。misere play では別の議論が必要。
5. 複数ゲームなら Grundy 数を加算せず XOR しているか。
