// P2895 流星雨 —— 独立参考解（不依赖“BFS 首次到达即最短”的思路）
// 做法：把 ti 看成未知量，反复松弛（Bellman-Ford 式）直到没有更新，
//       即 ti[v] = min(ti[u]+1)，约束是 ti[v] < b[v]。
// 网格截断到 N=40、坐标只生成 0..8 的小数据时足够用。
#include <bits/stdc++.h>
using namespace std;
const int N = 40;
const long long INF = (long long)4e18;

long long m;
long long dead[N][N];
long long f[N][N];
int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};

int main() {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) dead[i][j] = INF;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) f[i][j] = INF;

    if (!(cin >> m)) return 0;
    for (long long k = 0; k < m; ++k) {
        long long x, y, t;
        cin >> x >> y >> t;
        auto mark = [&](long long a, long long c) {
            if (a >= 0 && c >= 0 && a < N && c < N && t < dead[a][c]) dead[a][c] = t;
        };
        mark(x, y);
        for (int d = 0; d < 4; ++d) mark(x + dx[d], y + dy[d]);
    }

    // 起点能不能站：
    //   宽松（跟教材一致）：无条件 f[0][0] = 0；
    //   严格（物理上更对）：起点在 0 时刻就被砸中，人就没了 → -1。
    // 编译时加 -DSTRICT_START 切换，用来量化两种理解差多少。
#ifdef STRICT_START
    if (dead[0][0] > 0) f[0][0] = 0;
#else
    f[0][0] = 0;
#endif

    for (int iter = 0; iter < N * N; ++iter) {
        bool changed = false;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {
                if (f[i][j] == INF) continue;
                for (int d = 0; d < 4; ++d) {
                    int a = i + dx[d], c = j + dy[d];
                    if (a < 0 || c < 0 || a >= N || c >= N) continue;
                    if (f[i][j] + 1 >= dead[a][c]) continue;  // 到达时必须严格早于爆炸
                    if (f[i][j] + 1 < f[a][c]) {
                        f[a][c] = f[i][j] + 1;
                        changed = true;
                    }
                }
            }
        if (!changed) break;
    }

    long long ans = INF;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (dead[i][j] > 1000 && f[i][j] != INF) ans = min(ans, f[i][j]);

    if (ans == INF)
        cout << -1;
    else
        cout << ans;
    return 0;
}
