# 永続・部分永続・rollback の使い分け

## Path copying

更新で通る根から葉までのノードだけ複製し、変更しない部分木を以前の版と共有する。各版の根を保存すれば、全ての過去版を任意順に参照できる。

- 実装例: `data_structure/PersistentSegmentTree.cpp`
- 更新ごとの追加メモリ: `O(log N)`
- 適用先: 永続 Segment Tree、永続 Trie、永続配列

## Fat node

各フィールドに `{変更時刻, 値}` の履歴を持ち、過去時刻の値を二分探索する。更新は現在に対してだけ行い、過去版から分岐しない部分永続データ構造に向く。

- 実装例: `data_structure/PartiallyPersistentUnionFind.cpp`
- 注意: Union-Find の経路圧縮は履歴数を増やすため行わず、union by size だけを使う

## Rollback

更新前の差分を stack に積み、LIFO 順に取り消す。過去状態を任意順に参照することはできないが、分割統治や DFS と組み合わせると実装量が少ない。

- 実装例: `offline/RollbackOffline.cpp` の rollback Mo と時間軸分割統治
- 適用先: offline dynamic connectivity、時間つきクエリ、分割統治 DP

必要な履歴が木状に分岐するなら path copying、時刻が一直線なら fat node、探索順を設計できるなら rollback を優先する。
