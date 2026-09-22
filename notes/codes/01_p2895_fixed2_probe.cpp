// 探针：与 \test_\01_p2895_fixed2.cpp 完全相同，只多一个计数器
// 统计"加固版的边界判断拦掉了多少次本来会发生的越界访问"
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#ifndef GRID_N
#define GRID_N 310
#endif
const int N = GRID_N;
const int INF = 0x3f3f3f3f;

ll m = 0;
int rm[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
int b[N][N];
int ti[N][N];

struct dot {
    int x, y;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    memset(ti, -1, sizeof(ti));
    memset(b, 0x3f, sizeof(b));

    cin >> m;
    for (ll k = 0; k < m; ++k) {
        ll x, y, t;
        cin >> x >> y >> t;
        auto mark = [&](ll a, ll c) {
            if (a >= 0 && a < N && c >= 0 && c < N && t < b[a][c]) b[a][c] = (int)t;
        };
        mark(x, y);
        for (int d = 0; d < 4; ++d) mark(x + rm[d][0], y + rm[d][1]);
    }

    long long blockedNeg = 0, blockedHigh = 0, pushed = 0;
    queue<dot> q;
    q.push({0, 0});
    ti[0][0] = 0;
    while (!q.empty()) {
        dot u = q.front();
        q.pop();
        int t = ti[u.x][u.y];
        for (int d = 0; d < 4; ++d) {
            int nx = u.x + rm[d][0], ny = u.y + rm[d][1];
            if (nx < 0 || ny < 0) { ++blockedNeg; continue; }
            if (nx >= N || ny >= N) { ++blockedHigh; continue; }   // ★ 这一行就是新增的护栏
            if (ti[nx][ny] != -1) continue;
            if (t + 1 >= b[nx][ny]) continue;
            ti[nx][ny] = t + 1;
            ++pushed;
            q.push({nx, ny});
        }
    }

    int ans = INF;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (b[i][j] > 1000 && ti[i][j] != -1) ans = min(ans, ti[i][j]);

    fprintf(stderr, "[探针] 被下界拦下 %lld 次；被【上界】拦下 %lld 次（这些原来都是越界访问）\n",
            blockedNeg, blockedHigh);
    fprintf(stderr, "[探针] 实际入队格子数 %lld（N=%d，网格共 %d 格）\n", pushed, N, N * N);

    if (ans == INF) cout << -1;
    else cout << ans;
    return 0;
}
