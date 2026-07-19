//ワーシャルフロイドライブラリ
//隣接行列形式でデータを渡す
//負閉路の存在は想定されていないため、判定はされない
void warshallFloyd(vector<vector<long long>>& dist) {
    int N = (int)dist.size();
    for (int i = 0; i < N; i++) {
        dist[i][i] = 0;
    }
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) {
            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
        }
    }
}