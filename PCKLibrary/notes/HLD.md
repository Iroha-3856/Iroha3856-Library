```cpp
while(head[u] != head[v]) {
    if (in[u] > in[v]) swap(u, v);
    // v をあげる
    ans += seg.prod(in[head[v]], in[v] + 1);
    v = parent[head[v]];
}
if (in[u] > in[v]) swap(u, v);
```