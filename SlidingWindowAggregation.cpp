#include <bits/stdc++.h>
using namespace std;

template<class S, S(*op)(S, S)>
struct SlidingWindowAggregation {
    stack<pair<S, S>> stf, stb;
    SlidingWindowAggregation() : SlidingWindowAggregation(vector<S>{}) {}
    SlidingWindowAggregation(vector<S> A) {
        for (S a : A) push(a);
    } 
    void push(S x) {
        if (stb.empty()) {
            stb.push({x, x});
        }
        else {
            stb.push({x, op(stb.top().second, x)});
        }
    }
    void move() {
        if (stf.empty()) {
            while(!stb.empty()) {
                if (stf.empty()) {
                    stf.push({stb.top().first, stb.top().first});
                }
                else {
                    stf.push({stb.top().first, op(stb.top().first, stf.top().second)});
                }
                stb.pop();
            }
        }
    }
    void pop() {
        move();
        stf.pop();
    }
    S prod() {
        assert(!stf.empty() or !stb.empty());
        if (stf.empty()) return stb.top().second;
        if (stb.empty()) return stf.top().second;
        return op(stf.top().second, stb.top().second);
    }
    S prod(S e) {
        if (stf.empty() and stb.empty()) return e;
        if (stf.empty()) return stb.top().second;
        if (stb.empty()) return stf.top().second;
        return op(stf.top().second, stb.top().second);
    }
};