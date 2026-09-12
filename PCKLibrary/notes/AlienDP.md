# Alien DP / Lagrangian relaxation

$f(x)$ が凸な関数（傾きが単調減少）だとする。

$g(\lambda) = \max_x (f(x) - \lambda * x)$ とおくと、$g(\lambda)$ は $f(x)$ に傾き $\lambda$ の接線を引いたときの $y$ 切片の値と等しい。

ところで、$f(X) = \min_\lambda g(\lambda) + \lambda X$ であるので、$g(\lambda) + \lambda X$ が凹（傾きが単調増加）であることを利用すれば、ある値 $X$ での $f$ の評価値 $f(X)$ から計算できる。

凹関数の最小値を求めるにあたっては、傾きが負から正になる点を二分探索により求めるのがよい。

典型的な応用例：Monge d edge shortest path
