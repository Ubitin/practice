// 探针：完全照抄 \test_\01.cpp（你改后的版本）的 BFS，只额外统计"越界访问"次数
// 目的：量化 nx/ny >= 310 时对 b[310][ny] / ti[310][ny] 的越界读写
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = (ll)1e18;

ll m = 0;
ll rm[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
ll b[310][310];
ll ti[310][310];
ll pad[4096];   // 探针专用：如果 ti[310][*] 被写，就会落到 pad 上

struct dot {
    ll x, y;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    memset(ti, -1, sizeof(ti));
    for (ll i = 0; i < 310; ++i)
        for (ll j = 0; j < 310; ++j) b[i][j] = (ll)INF;
    for (int k = 0; k < 4096; ++k) pad[k] = 0;   // pad 清零，便于检测

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

    ll oobVisit = 0;     // 走到 nx/ny >= 310 的次数
    ll oobReadB = 0;     // 真的读了 b[310][*]
    ll oobBlocked = 0;   // 被判定拦下（没写）
    ll oobWrite = 0;     // ★ 真的写了 ti[310][*]（真正的危险）
    ll maxCoord = 0;

    queue<dot> q;
    q.push({0, 0});
    ti[0][0] = 0;
    while (!q.empty()) {
        dot u = q.front();
        q.pop();
        ll ux = u.x, uy = u.y;
        maxCoord = max(maxCoord, max(ux, uy));
        ll t = ti[ux][uy];
        for (ll i = 0; i < 4; ++i) {
            ll nx = ux + rm[i][0], ny = uy + rm[i][1];
            bool oob = (nx >= 310 || ny >= 310);
            if (oob) ++oobVisit;
            // 下面这一行的求值顺序与你的原码完全一致（|| 短路）
            if (nx < 0 || ny < 0) continue;
            if (oob) ++oobReadB;   // 执行到这里说明马上要读 b[310][*]
            if ((t + 1) >= b[nx][ny] || ti[nx][ny] != -1) {
                if (oob) ++oobBlocked;
                continue;
            }
            if (oob) ++oobWrite;
            ti[nx][ny] = t + 1;
            q.push({nx, ny});
        }
    }

    ll reach = 0, safeReach = 0;
    for (ll i = 0; i < 310; ++i)
        for (ll j = 0; j < 310; ++j)
            if (ti[i][j] != -1) {
                ++reach;
                if (b[i][j] > 1000) ++safeReach;
            }
    ll padTouched = 0;
    for (int k = 0; k < 4096; ++k)
        if (pad[k] != 0) ++padTouched;

    fprintf(stderr, "[探针] BFS 最远走到的坐标 = %lld（数组下标上限 309）\n", maxCoord);
    fprintf(stderr, "[探针] 撞到 nx/ny >= 310 的次数 = %lld\n", oobVisit);
    fprintf(stderr, "[探针]   其中真的读了 b[310][*]      = %lld\n", oobReadB);
    fprintf(stderr, "[探针]   被判据拦下（无写）        = %lld\n", oobBlocked);
    fprintf(stderr, "[探针] ★ 真的写了 ti[310][*]      = %lld\n", oobWrite);
    fprintf(stderr, "[探针] 越界写到 pad 上的元素个数   = %lld\n", padTouched);
    fprintf(stderr, "[探针] 可达格子 %lld 个，其中安全 %lld 个\n", reach, safeReach);

    ll ans = INF;
    for (ll i = 0; i < 310; ++i)
        for (ll j = 0; j < 310; ++j)
            if (b[i][j] > 1000 && ti[i][j] != -1) ans = min(ans, ti[i][j]);
    if (ans > 1000)
        cout << -1;
    else
        cout << ans;
    return 0;
}
