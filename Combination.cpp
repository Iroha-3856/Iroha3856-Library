struct Combination {
    vector<mint> fac, infac;
    Combination(int n) {
        fac.resize(n+1);
        infac.resize(n+1);
        fac[0] = 1;
        for (int i = 1; i <= n; i++) fac[i] = fac[i-1] * i;
        infac[n] = fac[n].inv();
        for (int i = n; i >= 1; i--) infac[i-1] = infac[i] * i;
    }
    mint operator()(int n, int k) {
        if (k < 0 || k > n) return 0;
        return fac[n] * infac[k] * infac[n-k];
    }
};