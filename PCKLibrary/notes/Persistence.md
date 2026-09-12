# 永続・部分永続・rollback の使い分け

同じ「過去の状態を使う」処理でも、参照したい履歴の形によって実装を選ぶ。

## Path copying

更新で通る根から葉までのノードだけ複製し、変更しない部分木を以前の版と共有する。
各版の根を保存すれば、すべての過去版を任意順に参照でき、過去版から新しい版を分岐できる。

- 実装例: `data_structure/PersistentSegmentTree.cpp`
- 更新時間・追加メモリ: 1 回あたり `O(log N)`
- 適用先: 永続 Segment Tree、永続 Trie、永続配列

```cpp
PersistentSegmentTree<ll, op, e> seg(N);
vector<int> root(Q + 1);
root[0] = seg.build(initial);
root[1] = seg.set(root[0], position, value);
root[2] = seg.set(root[0], anotherPosition, anotherValue); // 版 0 から分岐
ll answer = seg.prod(root[1], l, r);
```

## Fat node

各フィールドに `{変更時刻, 値}` の履歴を持ち、過去時刻の値を二分探索する。
更新は現在に対してだけ行い、過去版から分岐しない部分永続データ構造に向く。

- 実装例: `data_structure/PartiallyPersistentUnionFind.cpp`
- 使い方: `merge` 後の `now` を保存し、`same(u,v,time)` や `size(v,time)` を呼ぶ
- 注意: Union-Find の経路圧縮は履歴数を増やすため行わず、union by size だけを使う

```cpp
PartiallyPersistentUnionFind uf(N);
uf.merge(u, v);
int time = uf.now;
uf.merge(x, y);
bool connectedAtTime = uf.same(a, b, time);
```

## Rollback

更新前の差分を stack に積み、LIFO 順に取り消す。
過去状態を任意順に参照することはできないが、DFS や時間軸分割統治の探索順を設計できれば短く書ける。

- 実装例: `offline/RollbackOffline.cpp`
- snapshot は通常、履歴 stack の現在の大きさを返す
- rollback は stack が snapshot の大きさへ戻るまで差分を逆順に復元する
- 適用先: offline dynamic connectivity、時間つきクエリ、rollback Mo

```cpp
int snapshot() { return (int)history.size(); }
void rollback(int state) {
    while ((int)history.size() > state) undoOne();
}
```

## 選択基準

- 過去版からさらに更新して履歴を木状に分岐させるなら path copying。
- 時刻が一直線で、任意の過去時刻を問い合わせるなら fat node。
- 問い合わせ順をオフラインで並べ替えられ、更新を逆順に戻せるなら rollback。

完全永続性が不要なら、fat node や rollback の方が実装量とメモリを抑えやすい。
