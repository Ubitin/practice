// P2895 流星雨 —— 加固版（把 \test_\01.cpp 里唯一的一处 UB 彻底消掉）
//
// 与 \test_\01.cpp 的差别只有三点，都用 ★ 标出：
//   ★1 网格尺寸做成宏，默认 310（与教材一致）；万一测评数据坐标超过 309，改成 1010 再交一次
//   ★2 BFS 里补上"上界"判断（原来只判了 nx<0/ny<0）
//   ★3 输入侧也做边界保护（坐标越界时忽略该陨石，绝不对界外内存野写）
// 其余（严格早于摧毁时刻、入队即标记、ans 初值 INF、死区用 > 1000 判）与你现在的版本一致。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ★1 网格边长：题面 0 ≤ Xi,Yi ≤ 300，310 够用；若测评数据超出 309，把这里改成 1010
#ifndef GRID_N
#define GRID_N 310
#endif
const int N = GRID_N;
const int INF = 0x3f3f3f3f;   // 1061109567，远大于 t ≤ 1000；int 用 memset 0x3f 是合法的

ll m = 0;
int rm[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};   // 移动的四个方向
int b[N][N];    // 每个格子被摧毁的时间（INF = 永远不会爆炸）
int ti[N][N];   // 走到每个格子所需的最早时间（-1 = 未访问）

struct dot {
    int x, y;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    memset(ti, -1, sizeof(ti));   // -1 的每个字节都是 0xFF，memset 合法
    memset(b, 0x3f, sizeof(b));   // 每字节 0x3F → 每个 int = 0x3f3f3f3f（int 也可以这样用）

    cin >> m;
    for (ll k = 0; k < m; ++k) {
        ll x, y, t;
        cin >> x >> y >> t;
        // ★3 输入侧边界保护：坐标一旦超出网格，只忽略、不野写
        auto mark = [&](ll a, ll c) {
            if (a >= 0 && a < N && c >= 0 && c < N && t < b[a][c]) b[a][c] = (int)t;
        };
        mark(x, y);                                          // 落点
        for (int d = 0; d < 4; ++d)                          // 落点 + 上下左右一起摧毁
            mark(x + rm[d][0], y + rm[d][1]);
    }

    queue<dot> q;                 // 广度优先搜索的队列
    q.push({0, 0});
    ti[0][0] = 0;

    while (!q.empty()) {
        dot u = q.front();
        q.pop();
        int t = ti[u.x][u.y];
        for (int d = 0; d < 4; ++d) {
            int nx = u.x + rm[d][0], ny = u.y + rm[d][1];
            // ★2 四个方向都要判（原代码只判了 < 0，会去读 b[310][*]）
            if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
            if (ti[nx][ny] != -1) continue;          // 入队即标记
            if (t + 1 >= b[nx][ny]) continue;        // 必须严格早于摧毁时刻
            ti[nx][ny] = t + 1;
            q.push({nx, ny});
        }
    }

    int ans = INF;                                   // 求最小值：初值必须是"无穷大"
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (b[i][j] > 1000 && ti[i][j] != -1)    // 死区用区间判断
                ans = min(ans, ti[i][j]);

    if (ans == INF) cout << -1;                      // 判无解与初值成对
    else cout << ans;

    return 0;
}
