# PCK Library Contents

パソコン甲子園（予選・本選）で使用する C++ ライブラリの実装予定表。
持ち込み枚数に制限がない前提で、頻出事項を優先しつつ難しめのアルゴリズムまで収録する。

## 実装規約

このライブラリは、完成品をコピーするためではなく、競技中に見ながら短時間で写経するためのものとする。
以下を新規実装とリファクタリングの共通基準とする。

### 目的と収録基準

- 想定言語は C++17 以降とする。
- AtCoder 黄色帯を想定し、基本文法と STL だけで無思考に再実装できるものは収録しない。
- 短い処理でも、境界条件が多いものや高度な実装の共通部品になるものは収録する。
- 正しさを損なわない範囲で、汎用性よりも写経量と実装コストの小ささを優先する。
- 最小限の機能を本体とし、復元、特殊な初期化、用途の狭い補助 API は必要性が明確な場合だけ加える。
- 状態を保持するデータ構造やアルゴリズムは `struct`、単発の計算は関数または型エイリアスを基本とする。
- 返り値の意味が `pair`、`tuple`、`vector` で明白なら、結果を包むためだけの `struct` は作らない。
- `[A]` は最優先、`[B]` は余裕があれば、`[C]` は発展項目を表す。
- 各項目のチェックは実装完了を表す。コンパイル、ランダムテスト、オンラインジャッジでの検証状況とは分けて管理する。

### ファイル構成と依存

- 各 `.cpp` は共通テンプレートの後ろに写経する断片とし、原則として `#include`、`using namespace std;`、`main` を書かない。
- 共通テンプレートにある `ll`、`INF`、`chmin`、`chmax` などは再定義しない。
- 原則として 1 ファイルに 1 つのアルゴリズムまたはデータ構造を置く。ただし幾何の基本演算など、分離すると使いにくい一群はまとめてよい。
- ファイル名と主要な型名は一致させ、`PascalCase` とする。数値型による別実装は `_integer`、`_double` のような接尾辞で区別してよい。
- `[A]` 同士の依存は許可し、同じ処理を重複実装しない。依存がある場合はファイル先頭に `//依存: math/ExtGCD.cpp` の形式で書く。
- 循環依存は作らない。依存先を含める順序だけで使用できる構成にする。
- 整数幾何と浮動小数点幾何のように識別子が衝突する実装は、同時に使用しないことを各ファイルに明記する。
- 解説だけを残す項目は `notes/` 以下の Markdown に置き、実装ファイルと区別する。

### 書式

- インデントは半角スペース 4 個とし、タブは使わない。
- 開き波括弧は宣言または制御文と同じ行に置く。
- `if (`、`for (`、`while (` のように制御構文の直後へ空白を入れる。関数呼び出しは `f(x)` とし、関数名と `(` の間には入れない。
- コンマの後ろと二項演算子の前後には空白を入れる。例: `min(a, b)`、`l + r`、`x == y`、`l >> i`。
- 単項演算子、添字、メンバアクセスの内側には空白を入れない。例: `-x`、`A[i]`、`p.first`。
- 論理積と論理和は、既存ライブラリに合わせて `and`、`or` を用いる。
- テンプレート宣言は `template<class T>`、参照は `const vector<T>& A` の形にする。
- コンテナの大きさを `int` に直す場合は `(int)A.size()` の形で明示する。
- 対称で短い更新は `l += siz; r += siz;` のように同じ行へ置いてよい。それ以外は原則として 1 行 1 文とする。
- 意味のまとまりの間には空行を 1 行入れる。位置合わせだけを目的とした余分な空白や、連続した空行は入れない。
- 1 行が過度に長くなる場合は、引数または式の意味の切れ目で改行し、継続行を 4 スペース以上深くする。

```cpp
template<class T>
T rangeSum(const vector<T>& A, int l, int r) {
    T ret = 0;
    for (int i = l; i < r; i++) {
        ret += A[i];
    }
    return ret;
}
```

### 命名

- ファイル名、`struct`、`using` で定義する主要な型は `PascalCase` とする。
- 関数とメンバ関数は `lowerCamelCase` とする。競技で定着した `dfs`、`bfs`、`lca`、`ccw` などはそのままでよい。
- テンプレート型は `T`、`S`、`F`、`DP` などの短い大文字を用いる。
- 頂点数や列数は `N`、グラフは `G`、無限大は `INF` とする。二冪へ切り上げた内部サイズは `siz` とし、元の要素数と区別する。
- 区間の左端と右端は `l`、`r`、頂点は `v`、遷移先は `to` を基本とする。数式に対応する局所変数は短い名前でよい。
- 公開 API には、競技で一般的でない独自の略語を使わない。
- 同じ概念にはファイルをまたいで同じ名前を使う。異なる意味の値へ、既存の慣例名を流用しない。

### 型、添字、数値計算

- 頂点番号、添字、要素数は原則 `int`、距離、コスト、場合の数など値域が大きい整数は原則 `ll` とする。
- 添字は 0-indexed、区間は半開区間 `[l, r)` を原則とする。例外は API の先頭コメントに明記する。
- 到達不能は `INF`、負閉路の影響下など負方向へ発散する値は `-INF` で表す。`INF` または `-INF` に対して加減算しない。
- 積だけが `ll` を超える可能性がある場合は、中間計算に `__int128_t` を用いる。最終結果も収まらない実装は、許容範囲をコメントに書く。
- 剰余は原則 `[0, mod)` に正規化する。`mod > 0`、法が素数などの前提条件は省略しない。
- 浮動小数点幾何は `long double` と `EPS`、整数幾何は整数型と符号判定を使い、両者の比較方法を混在させない。
- 空配列、空区間、単一要素、非連結グラフを受け取れるかを決め、受け取れない場合だけ `assert` で契約を示す。

### API と実装

- 読み取り専用の大きな引数は `const T&` で受け取る。所有権の移動を前提とする API は作らない。
- 初期値から構築するのが自然なデータ構造はコンストラクタで構築する。更新は状態を変更するメンバ関数、参照だけの処理は可能なら `const` メンバ関数にする。
- API は競技中に必要な操作へ絞る。内部実装を露出するアクセサや、1 行で代用できるラッパーは追加しない。
- 失敗、解なし、未到達を返す場合は、`-1`、空配列、`INF` などの表現を先頭コメントで固定する。
- モノイド、作用、行列積など順序が重要な処理は、非可換でも壊れない順序で書き、合成の向きをコメントまたは使用例で示す。
- 汎用テンプレートには、具体的な型と演算を当てはめた最小の使用例を付ける。
- 再帰の深さが入力サイズに比例する場合は、反復実装で写経量が大きく増えない限り反復実装を優先する。
- `assert` は呼び出し側の契約確認にだけ使い、入力値の通常の分岐処理の代わりには使わない。
- 乱択を使う実装には、衝突または失敗の可能性と、必要なら決定的な代替手段を記載する。

### コメントと検証

- ファイル先頭に用途、計算量、添字と区間、前提条件を簡潔に書く。
- 必要に応じて、空入力、番兵値、オーバーフロー、精度、破壊的変更の有無も先頭に書く。
- コメントはコードを日本語へ言い換えるのではなく、式の向き、境界条件、実装上の理由を説明する。
- 写経時に確認しやすいよう、長い証明や一般論は実装へ埋め込まず、必要なら `notes/` に分離する。
- 完了した実装は、共通テンプレートと合わせて C++17 で警告なくコンパイルできる状態にする。
- 検証では、まず空・最小・最大付近・非連結などの境界例を確認し、その後に愚直解とのランダムテストとオンラインジャッジの verify を行う。

## 0. 共通部品

- [x] `[B]` 小さな便利関数集 — `utility/SafeInteger.cpp`
  - floor division / ceil division（負数対応）
  - `__int128` を用いた安全な掛け算・大小比較

## 1. データ構造

- [x] `[B]` Disjoint Sparse Table — `data_structure/DisjointSparseTable.cpp`
  - 結合則だけを仮定した静的区間積、非可換演算
- [x] `[B]` SWAG（Sliding Window Aggregation）— `data_structure/SlidingWindowAggregation.cpp`
  - queue / deque 上のモノイド積、非可換演算の積順序
- [x] `[B]` Binary Trie — `data_structure/BinaryTrie.cpp`
  - XOR 最小・最大、全体 XOR、個数管理
- [x] `[B]` Treap / Randomized BST — `data_structure/ImplicitTreap.cpp`
  - split / merge、列への挿入・削除・反転
- [x] `[C]` Segment Tree Beats — `data_structure/SegmentTreeBeats.cpp`
  - range chmin / chmax / add / sum、計算量の償却条件
  - range chmin / chmax / add / sum に限らない場合の一般化した実装方法をコメントに書く
- [x] `[C]` 永続 Segment Tree — `data_structure/PersistentSegmentTree.cpp`
  - バージョン管理、区間 k 番目
- [x] `[C]` 部分永続 Union-Find — `data_structure/PartiallyPersistentUnionFind.cpp`
  - 過去時刻の連結性・連結成分サイズ、rollback 版との使い分け
- [x] `[C]` Li Chao Tree — `data_structure/LiChaoTree.cpp`
  - 直線追加・最小値、座標圧縮版、区間への直線追加
- [x] `[C]` Wavelet Matrix — `data_structure/WaveletMatrix.cpp`
  - `rank`、`kth`、区間頻度、前後の値

## 2. グラフ

- [x] `[B]` 強連結成分分解（SCC）— `graph/StronglyConnectedComponents.cpp`
  - 縮約 DAG の構築
- [x] `[B]` 2-SAT — `graph/TwoSAT.cpp`
- [x] `[B]` 橋・関節点 — `graph/LowLink.cpp`
  - low-link、二重辺に対応
- [x] `[B]` 二辺連結成分分解・二重頂点連結成分分解 — `graph/BiconnectedComponents.cpp`
  - bridge tree、block-cut tree の構築
- [x] `[B]` オイラー路・オイラー閉路 — `graph/EulerTrail.cpp`
  - 有向・無向、存在条件、辺列の復元
- [x] `[B]` 補グラフの連結成分分解 — `graph/ComplementGraphComponents.cpp`
  - 未訪問頂点集合を用いる `O(N + M log N)` 程度の実装
- [x] `[B]` Steiner Tree DP — `graph/SteinerTree.cpp`
  - ターミナル部分集合 DP + 最短路、復元の要否を分離

## 3. 木

- [x] `[A]` Lowest Common Ancestor（ダブリング）— `tree/LowestCommonAncestor.cpp`
  - LCA、距離、パス上の k 番目
- [x] `[A]` Lowest Common Ancestor（Sparse Table）— `tree/LowestCommonAncestorSparseTable.cpp`
  - Euler Tour + RMQ、構築 `O(N log N)`、LCA `O(1)`
- [x] `[A]` rerooting DP（全方位木 DP）— `tree/Rerooting.cpp`
  - モノイド形式、距離和を求める使用例をコメントに収録
- [x] `[B]` Heavy-Light Decomposition — `tree/HeavyLightDecomposition.cpp`
  - パス・部分木を半開区間へ分解
  - 頂点版、辺版、非可換演算への注意
- [x] `[B]` Cartesian Tree — `tree/CartesianTree.cpp`
  - 親配列、最小値版・最大値版、同値要素の tie-break
- [x] `[B]` DSU on Tree / Sack — `tree/DSUOnTree.cpp`
  - small-to-large、部分木クエリ、状態の追加と削除
- [x] `[B]` 木同型判定・木ハッシュ — `tree/TreeHash.cpp`
  - 根付き・根なし、中心、乱択 hash の衝突可能性
- [x] `[B]` 重心分解 — `tree/CentroidDecomposition.cpp`
  - 重心分解木を返す構造にする
- [x] `[B]` Auxiliary Tree / Virtual Tree — `tree/VirtualTree.cpp`
- [x] `[C]` Static Top Tree — `tree/StaticTopTree.cpp`
  - 木 DP の一点更新、cluster の境界と merge の向き

## 4. フロー・マッチング

- [x] `[A]` 最大流 — `flow/FordFulkerson.cpp`, `flow/Dinic.cpp`
  - 短い Ford–Fulkerson と、高速な Dinic を制約で使い分ける
  - 最小カット側の頂点集合を残余グラフから復元する
- [x] `[A]` 二部マッチング — `flow/BipartiteMatching.cpp`
  - Kuhn 法、マッチした左右の頂点対を復元
- [x] `[B]` 二部グラフの最小頂点被覆・最大独立集合 — `flow/BipartiteVertexCover.cpp`
  - 最大マッチング後の交互路から復元する
- [x] `[B]` 最小費用流 — `flow/MinCostFlow.cpp`
  - ポテンシャル + Dijkstra、負辺対応、流量指定
- [x] `[B]` lower-bound flow — `flow/LowerBoundFlow.cpp`
  - 辺の流量下限制約、実行可能流、s-t flow
- [x] `[B]` Project Selection Problem / Maximum Closure — `flow/ProjectSelection.cpp`
  - 利得・罰金を s-t cut へ変換する典型をメモする
- [x] `[B]` Hungarian Algorithm — `flow/Hungarian.cpp`
  - 長方形コスト行列、最小重み完全マッチング、割当の復元

## 5. 数学

### 5.1 整数・組合せ

- [x] `[A]` 拡張 Euclid — `math/ExtGCD.cpp`
  - `gcd(a, b)` と係数、一次不定方程式、mod 逆元
- [x] `[A]` Euler の φ 関数と約数包除のメモ — `notes/EulerPhi.md`
  - 積公式、φ の篩、gcd ごとの分類、Möbius反転
- [x] `[A]` modint — `math/Modint.cpp`
  - 静的 mod、四則演算、逆元、負数正規化
- [x] `[A]` 中国剰余定理（CRT）— `math/CRT.cpp`
  - 非互いに素な法、解なし判定
- [x] `[B]` Lucas の定理 — `math/Lucas.cpp`
- [x] `[B]` 任意 mod の二項係数 — `math/ArbitraryModBinomial.cpp`
- [x] `[B]` 包除原理・メビウス変換 — `math/ZetaMobiusTransform.cpp`
  - divisor / multiple zeta transform、subset zeta transform
- [x] `[B]` floor sum — `math/FloorSum.cpp`
- [x] `[B]` 商列挙（quotient enumeration）— `math/EnumerateQuotients.cpp`
  - `floor(N / i)` が一定の区間、調和級数分割
- [x] `[B]` XOR basis（GF(2) 線形基底）— `math/XorBasis.cpp`
  - 表現可能判定、最大 XOR、k 番目
- [x] `[B]` Stern–Brocot Tree / 有理数近似 — `math/SternBrocotTree.cpp`
  - Euclid 法による run-length 経路表現、分母制限つき近似
- [x] `[C]` Miller-Rabin 素数判定（64 bit 決定的）— `math/NumberTheory64.cpp`
- [x] `[C]` Pollard's rho 素因数分解 — `math/NumberTheory64.cpp`
- [x] `[C]` 離散対数（Baby-step Giant-step）— `math/DiscreteLogarithm.cpp`
- [x] `[C]` 原始根・mod 平方根（Tonelli–Shanks）・離散 k 乗根 — `math/ModularRoot.cpp`
- [x] `[C]` 素数計数・乗法的関数の和 — `math/PrimeSummatory.cpp`
  - Lucy DP / Min_25 sieve、商列挙との依存を明記する

### 5.2 線形代数・多項式

- [x] `[A]` 行列 — `math/Matrix.cpp`
  - 積、単位行列、繰り返し二乗法による累乗
  - mod 行列、連立漸化式への適用
- [x] `[B]` Gauss-Jordan 消去法 — `math/GaussJordan.cpp`
  - 実数、mod、GF(2) bitset、解空間
- [x] `[C]` FWT — `math/FWT.cpp`
  - XOR / AND / OR convolution
- [x] `[C]` Lagrange 補間 — `math/LagrangeInterpolation.cpp`
  - 1 点での評価、標本点が連続整数の場合の `O(N)` 評価

## 6. 文字列

- [x] `[A]` Rolling Hash — `string/RollingHash.cpp`
  - `unsigned long long` の自然なオーバーフローを使う単一 hash
- [x] `[A]` Z algorithm — `string/ZAlgorithm.cpp`
  - パターン検索、文字列の周期、境界をまたぐ一致長
- [x] `[A]` KMP / prefix function — `string/KMP.cpp`
  - パターン検索、failure link、最小周期
- [x] `[A]` Trie — `string/Trie.cpp`
  - 挿入、完全一致、通過回数
- [x] `[B]` Aho-Corasick 法 — `string/AhoCorasick.cpp`
  - 複数パターン検索、出現回数の伝播
- [x] `[B]` Manacher 法 — `string/Manacher.cpp`
  - 奇数長・偶数長回文
- [x] `[B]` Suffix Array + LCP Array — `string/SuffixArray.cpp`
  - 部分文字列検索、LCP の RMQ
- [x] `[C]` Run 列挙 — `string/RunEnumeration.cpp`

## 7. 計算幾何

- [x] `[A]` 2 次元 Point / Vector
  - 実数版 `geometry/Geometry2D_double.cpp`: `complex<long double>`、dot、cross、回転
  - 整数版 `geometry/Geometry2D_integer.cpp`: `long long` 座標と四則演算
- [x] `[A]` ccw / orientation — 実数版・整数版の両方
- [x] `[A]` 線分交差判定 — 実数版・整数版の両方
  - 端点・一直線・退化線分を含む
- [x] `[A]` 直線・線分・点の距離 — `geometry/Geometry2D_double.cpp`
- [x] `[A]` 交点 — `geometry/Geometry2D_double.cpp`
  - 直線同士、円と直線、円同士
- [x] `[A]` 多角形 — 実数版・整数版の両方
  - 符号付き面積、凸性、点包含、格子点の Pick の定理
- [x] `[A]` 凸包（Andrew's monotone chain）— 実数版・整数版の両方
  - 一直線上の点を残す版・除く版
- [x] `[B]` 凸多角形 — `geometry/ConvexPolygon.cpp`
  - 点包含、直径（rotating calipers）、convex cut、接線
- [x] `[B]` Minkowski sum — `geometry/MinkowskiSum.cpp`
  - 凸多角形の辺ベクトル merge、退化辺と始点の正規化
- [x] `[B]` 最近点対 — `geometry/ClosestPair.cpp`
- [x] `[B]` 円 — `geometry/Circle.cpp`
  - 外接円、内接円、共通接線、円の共通部分面積
- [x] `[B]` 最小包含円 — `geometry/MinimumEnclosingCircle.cpp`
  - 乱択 incremental、共線・重複点、失敗確率
- [x] `[C]` Manhattan MST — `geometry/ManhattanMST.cpp`
  - 8 方向の候補辺列挙 + Kruskal

## 8. 探索・最適化

- [x] `[B]` Knuth optimization — `optimization/KnuthOptimization.cpp`
  - 四辺形不等式と区間包含単調性、最適分割点の復元
- [x] `[B]` Divide and Conquer DP optimization — `optimization/DivideAndConquerDP.cpp`
  - argmin の単調性、前の DP 行から次の行を計算する形
- [x] `[B]` Convex Hull Trick — `optimization/ConvexHullTrick.cpp`
  - 傾き単調、クエリ単調、Li Chao Tree との使い分け
- [x] `[B]` Monotone Minima — `optimization/MonotoneMinima.cpp`
  - 行ごとの argmin が単調な行列、D&C DP の共通部品
- [x] `[C]` SMAWK — `optimization/SMAWK.cpp`
  - totally monotone 行列の各行最小値、Monge との包含関係
- [x] `[B]` noshi式簡易LARSCH — `optimization/SimpleLarsch.cpp`
  - Monge 型オンライン DP、適用条件と通常の LARSCH との差
- [x] `[C]` LARSCH — `optimization/Larsch.cpp`
  - totally monotone な下三角行列上のオンライン行最小値
- [x] `[B]` Slope Trick — `optimization/SlopeTrick.cpp`
  - 区分線形凸関数、左右 heap、平行移動と累積最小化
- [x] `[B]` convex / concave min-plus convolution — `optimization/MinPlusConvolution.cpp`
  - 一方または両方が凸・凹な場合を分け、計算量を明記する
- [x] `[C]` Alien DP / Lagrangian relaxation — `notes/AlienDP.md`
  - Monge d 辺最短路がどのように導かれるかを note にする
  - 違反回数によるものではなく、凸性による実装を基本として実装例も記載する

## 9. オフライン処理・高度なクエリ

- [x] `[B]` Mo's algorithm — `offline/Mo.cpp`
  - add / erase、クエリ順序、更新つき Mo、木上 Mo
- [x] `[C]` rollback Mo / divide and conquer on time — `offline/RollbackOffline.cpp`
  - 削除しにくい状態と可逆な更新を扱う
- [x] `[C]` 永続・部分永続データ構造の共通パターン — `notes/Persistence.md`

## 10. ゲーム・その他

- [x] `[A]` Grundy 数の典型メモ — `notes/Grundy.md`
  - DAG 上のゲーム、mex、複数ゲームの XOR
  - 状態が大きい場合の周期性、勝敗だけを求める場合との使い分け

## 11. 実装品質と検証

各ライブラリは次を満たしてから完了扱いにする。

- [ ] API と添字規約がコメントされている。
- [ ] 時間・空間計算量がコメントされている。
- [ ] 空入力、要素数 1、重複、非連結などの境界値を試した。
- [ ] `int` の積が先にオーバーフローしない。
- [ ] `INF + cost`、符号付き整数のシフト、負数の剰余を確認した。
- [ ] 乱択テストで愚直解と比較した。
- [ ] AOJ などの提出可能な検証問題を最低 1 問通した。
- [ ] 依存するファイルと必要な標準ヘッダを明記した。
- [ ] 印刷時にもコードと説明がページをまたいで崩れない。

## 推奨ディレクトリ構成

```text
.
├── contents.md
├── utility/
├── data_structure/
├── graph/
├── tree/
├── flow/
├── math/
├── string/
├── geometry/
├── optimization/
├── offline/
├── notes/
└── verify/
```

## 実装順

1. `[A]` のデータ構造、グラフ、木、整数、文字列、幾何を揃える。
2. `[A]` の各実装を過去問とランダムテストで検証する。
3. 最大流・SCC・HLD・FFT など `[B]` の汎用性が高いものを追加する。
4. 過去問演習で実際に欲しくなった API や典型を追記する。
5. `[C]` は写経せず、検証済みコードと使用条件をセットで収録する。

実戦では、未検証の高度な実装よりも、短く使い慣れた `[A]` の実装を優先する。
