template<class T>
T safe_mod(T x, T y) {
    assert(y > 0);
    T ret = x % y;
    if (ret < 0) ret += y;
    return ret;
}

template<class T>
T floor(T x, T y) {
    assert(y != 0);
    T q = x / y;
    T r = x % y;
    if (r != 0 && ((r > 0) != (y > 0))) q--;
    return q;
}

template<class T>
T ceil(T x, T y) {
    assert(y != 0);
    T q = x / y;
    T r = x % y;
    if (r != 0 && ((r > 0) == (y > 0))) q++;
    return q;
}
