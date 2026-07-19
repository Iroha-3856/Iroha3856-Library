template<class T>
T isqrt(T a) {
    for (T x = sqrt(a)+2; ; x--) {
        if (x * x <= a) return x;
    }
}