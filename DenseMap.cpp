#include <ankerl/unordered_dense.h>

template<class K, class V>
using DenseMap = ankerl::unordered_dense::map<K, V>;
template<class T>
using DenseSet = ankerl::unordered_dense::set<T>;

int main() {
    DenseMap<int, int> mp;
}