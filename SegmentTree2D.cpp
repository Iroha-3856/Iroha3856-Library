#include<bits/stdc++.h>
using namespace std;

template<class T, T(*op)(T, T), T(*e)()>
struct SegmentTree2D {
    int H, W;
    vector<vector<T>> data;
    SegmentTree2D(int h, int w) : SegmentTree2D(vector<vector<T>>(h, vector<T>(w, e()))) {}

    int id(int r, int c) { return r * 2 * W + c; }

    SegmentTree2D(vector<vector<T>> A, bool do_build = true) {
        int H = 1, W = 1;
        while(H < h) H <<= 1;
        while(W < w) W <<= 1;
        data.resize(4*H*W);
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                data[id(i+H, j+W)] = A[i][j];
            }
        }
        if (do_build) build();
    }
    
    void build() {
        for (int i = H-1; i >= 1; i--) {
            for (int j = W; j < 2*W; j++) {
                data[i][j] = op(data[id(i<<1, j)][j], data[id(i<<1|1, j)]);
            }
        }
        for (int i = 1; i < 2*H; i++) {
            for (int j = W-1; j >= 0; j--) {
                data[i][j] = op(data[i][id(i, j<<1)], data[i][id(i, j<<1|1)]);
            }
        }
    }

    T rowProd(int row, int l, int r) {
        l += W; r += W;
        T ret = e();
        while(l < r) {
            if (l&1) ret = op(ret, data[row][l++]);
            if (r&1) ret = op(ret, data[row][--r]);
            l >>= 1; r >>= 1; 
        }
        return ret;
    }

    //[rowl, rowr)[columnl, columnr)
    T prod(int rowl, int rowr, int columnl, int columnr) {
        assert(rowl <= rowr); assert(columnl <= columnr);
        T ret = e();
        while(rowl < rowr) {
            if (rowl&1) { ret = op(ret, rowProd(rowl, columnl, columnr)); rowl--; }
            if (rowr&1) { rowr--; ret = op(ret, rowProd(rowr, columnl, columnr)); }
        }
        return ret;
    }
};