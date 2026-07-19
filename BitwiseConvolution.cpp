std::vector<mint> xorConvolution(std::vector<mint> A, std::vector<mint> B) {
    int N = (int)A.size();
    assert((int)B.size() == N);
    assert((N&(N-1)) == 0);

    auto transform = [&N](std::vector<mint>& X) -> void {
        for (int i = 1; i < N; i <<= 1) {
            for (int j = i; j < N; j = (j + 1) | i) {
                mint s = X[j^i], t = X[j];
                X[j^i] = s + t;
                X[j] = s - t;
            }
        }
    };
    auto inverse = [&N, &transform](std::vector<mint>& X) -> void {
        transform(X);
        mint inv = mint(N).inv();
        for (mint& x : X) x *= inv;
    };
    
    transform(A); transform(B);
    for (int i = 0; i < N; i++) A[i] *= B[i];
    inverse(A);
    return A;
}

std::vector<mint> andConvolution(std::vector<mint> A, std::vector<mint> B) {
    int N = (int)A.size();
    assert((int)B.size() == N);
    assert((N&(N-1)) == 0);

    auto transform = [&N](std::vector<mint>& X) -> void {
        for (int i = 1; i < N; i <<= 1) {
            for (int j = i; j < N; j = (j + 1) | i) {
                X[j^i] += X[j];
            }
        }
    };
    auto inverse = [&N](std::vector<mint>& X) -> void {
        for (int i = 1; i < N; i <<= 1) {
            for (int j = i; j < N; j = (j + 1) | i) {
                X[j^i] -= X[j];
            }
        }
    };
    
    transform(A); transform(B);
    for (int i = 0; i < N; i++) A[i] *= B[i];
    inverse(A);
    return A;
}

std::vector<mint> orConvolution(std::vector<mint> A, std::vector<mint> B) {
    int N = (int)A.size();
    assert((int)B.size() == N);
    assert((N&(N-1)) == 0);

    auto transform = [&N](std::vector<mint>& X) -> void {
        for (int i = 1; i < N; i <<= 1) {
            for (int j = i; j < N; j = (j + 1) | i) {
                X[j] += X[j^i];
            }
        }
    };
    auto inverse = [&N](std::vector<mint>& X) -> void {
        for (int i = 1; i < N; i <<= 1) {
            for (int j = i; j < N; j = (j + 1) | i) {
                X[j] -= X[j^i];
            }
        }
    };
    
    transform(A); transform(B);
    for (int i = 0; i < N; i++) A[i] *= B[i];
    inverse(A);
    return A;
}