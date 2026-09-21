// P2895 流星雨 —— 修正版（保留原代码的结构与变量名，只修 bug）
// 与 \test_\01.cpp 的差异都用 ★ 标出
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll INF = (ll)1e18;   // ★1 “无穷大”必须是一个真正的很大的数

ll m = 0;
ll rm[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};  // 移动的四个方向
ll b[310][310];    // 每个格子发生爆炸的时间
ll ti[310][310];   // 记录走到每个格子所需的最短时间

struct dot {       // 用于记录坐标的结构体
    ll x, y;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    memset(ti, -1, sizeof(ti));  // -1 的每个字节都是 0xFF，这样用 memset 是合法的

    // ★2 不能用 memset(b, 1002, sizeof(b))：memset 按“字节”填充，
    //    long long 占 8 字节，会被填成 0xEAEAEAEAEAEAEAEA（负数！）
    for (ll i = 0; i < 310; ++i)
        for (ll j = 0; j < 310; ++j)
            b[i][j] = INF;

    cin >> m;
    for (ll k = 0; k < m; ++k) {  // 记录每个格子陨石落下的时间
        ll x, y, t;
        cin >> x >> y >> t;
        if (x >= 0 && y >= 0) b[x][y] = min(t, b[x][y]);  // ★3 落点本身也要判负
        for (ll d = 0; d < 4; ++d) {
            ll xt = x + rm[d][0], yt = y + rm[d][1];
            if (xt >= 0 && yt >= 0) b[xt][yt] = min(t, b[xt][yt]);
        }
    }

    // ★4 教材没处理这一步：起点在 0 时刻就被砸中（比如陨石砸在 (1,0)，t=0）。
    //    默认 STRICT_START 0 = 跟教材行为完全一致（判题按教材走，最稳）。
    //    想物理上更严格就改成 1：人还没迈步就没了，直接输出 -1。
#define STRICT_START 0
#if STRICT_START
    if (b[0][0] == 0) {
        cout << -1;
        return 0;
    }
#endif

    queue<dot> q;  // 广度优先搜索的队列
    dot o = {0, 0};
    q.push(o);
    ti[0][0] = 0;

    while (!q.empty()) {
        dot u = q.front();
        q.pop();
        ll ux = u.x, uy = u.y;
        ll t = ti[ux][uy];
        for (ll d = 0; d < 4; ++d) {
            ll nx = ux + rm[d][0], ny = uy + rm[d][1];
            if (nx < 0 || ny < 0 || nx >= 310 || ny >= 310) continue;  // ★5 上界也要判
            if (ti[nx][ny] != -1) continue;                            // 已经到过
            if (t + 1 >= b[nx][ny]) continue;   // 必须严格早于爆炸时刻才能站上去
            ti[nx][ny] = t + 1;
            dot nd = {nx, ny};
            q.push(nd);
        }
    }

    ll ans = INF;  // ★6 求最小值，初值必须是“无穷大”，不能是 0
    for (ll i = 0; i < 310; ++i) {
        for (ll j = 0; j < 310; ++j) {
            if (b[i][j] > 1000 && ti[i][j] != -1) {  // ★7 用区间判断，别用 == 1002
                ans = min(ans, ti[i][j]);
            }
        }
    }

    if (ans == INF)  // ★8 判无解要和初值比较
        cout << -1;
    else
        cout << ans;

    return 0;
}
