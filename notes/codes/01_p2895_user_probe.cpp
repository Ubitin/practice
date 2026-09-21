// 探针：把 \test_\01.cpp 的逻辑原样跑一遍，额外统计“走了多少格子”
// 用来证明 BFS 是否真的被 b[][] 的初值卡死在起点
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll m = 0;
ll rm[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
ll b[310][310];
ll ti[310][310];

struct dot {
    ll x, y;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    memset(ti, -1, sizeof(ti));
    memset(b, 1002, sizeof(b));  // ← 可疑的那一行

    ll cnt1002 = 0, cntnonpos = 0;
    for (ll i = 0; i < 310; ++i)
        for (ll j = 0; j < 310; ++j) {
            if (b[i][j] == 1002) ++cnt1002;
            if (b[i][j] <= 0) ++cntnonpos;
        }
    fprintf(stderr, "[探针] b 数组里等于 1002 的格子数 = %lld\n", cnt1002);
    fprintf(stderr, "[探针] b 数组里 <= 0 的格子数     = %lld\n", cntnonpos);
    fprintf(stderr, "[探针] b[0][0] = %lld  b[0][0]==1002? %s\n", b[0][0],
            b[0][0] == 1002 ? "是" : "否");

    cin >> m;
    for (ll i = 0; i < m; ++i) {
        ll x, y, t;
        cin >> x >> y >> t;
        b[x][y] = min(t, b[x][y]);
        for (ll i = 0; i < 4; ++i) {
            ll xt = x + rm[i][0], yt = y + rm[i][1];
            if (xt >= 0 && yt >= 0) b[xt][yt] = min(t, b[xt][yt]);
        }
    }
    fprintf(stderr, "[探针] 读入陨石后 b[0][0] = %lld  (原始时刻是 2)\n", b[0][0]);

    queue<dot> q;
    dot o = {0, 0};
    q.push(o);
    ti[0][0] = 0;
    ll popped = 0, expanded = 0;
    while (!q.empty()) {
        dot u = q.front();
        q.pop();
        ++popped;
        ll ux = u.x, uy = u.y;
        ll t = ti[ux][uy];
        for (ll i = 0; i < 4; ++i) {
            ll nx = ux + rm[i][0], ny = uy + rm[i][1];
            if (nx < 0 || ny < 0 || (t + 1) >= b[nx][ny] || ti[nx][ny] != -1) continue;
            ti[nx][ny] = t + 1;
            ++expanded;
            dot d = {nx, ny};
            q.push(d);
        }
    }
    ll reach = 0, safeReach = 0;
    for (ll i = 0; i < 310; ++i)
        for (ll j = 0; j < 310; ++j)
            if (ti[i][j] != -1) {
                ++reach;
                if (b[i][j] == 1002) ++safeReach;
            }
    fprintf(stderr, "[探针] BFS 出队次数 = %lld, 成功扩展次数 = %lld\n", popped, expanded);
    fprintf(stderr, "[探针] 可达格子数 = %lld, 其中“安全且可达” = %lld\n", reach, safeReach);
    fprintf(stderr, "[探针] 答案变量 ans 的初值是 0, 循环里 min(0, 任何非负数) 恒为 0\n");

    ll ans = 0;
    for (ll i = 0; i < 310; ++i)
        for (ll j = 0; j < 310; ++j)
            if (b[i][j] == 1002 && ti[i][j] != -1) ans = min(ans, ti[i][j]);
    fprintf(stderr, "[探针] 最终 ans = %lld, (ans==1002)? %s\n", ans, ans == 1002 ? "是" : "否");
    if (ans == 1002)
        cout << -1;
    else
        cout << ans;
    return 0;
}
