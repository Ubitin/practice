// dfs_param_cnt_bug.cpp —— 反例：后序名单只开 maxn=502 够不够？
// 除了一行数组大小，其余和 dfs_param_demo.cpp 完全一样。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll maxn = 502;

ll n, m, k;
bool wall[maxn][maxn];
bool vis[maxn][maxn];
ll timer = 0;
struct dot { ll x, y; };
dot ord[maxn];                 // ←←← 这里只开 502，装不下 12.5 万个空格

const ll dx[4] = {0, 0, 1, -1};
const ll dy[4] = {1, -1, 0, 0};

void dfs(ll x, ll y) {
    vis[x][y] = true;
    for (ll d = 0; d < 4; ++d) {
        ll nx = x + dx[d], ny = y + dy[d];
        if (nx < 1 || nx > n || ny < 1 || ny > m) continue;
        if (wall[nx][ny] || vis[nx][ny]) continue;
        dfs(nx, ny);
    }
    ord[++timer] = {x, y};     // 写越界：timer 一超过 502 就在踩别人的内存
}

int main() {
    if (scanf("%lld %lld %lld", &n, &m, &k) != 3) return 1;
    ll sx = -1, sy = -1, blank = 0;
    for (ll i = 1; i <= n; ++i) {
        static char buf[maxn];
        scanf("%s", buf + 1);
        for (ll j = 1; j <= m; ++j) {
            wall[i][j] = (buf[j] == '#');
            if (!wall[i][j]) { ++blank; if (sx < 0) { sx = i; sy = j; } }
        }
    }
    dfs(sx, sy);
    printf("空格总数 blank = %lld\n", blank);
    printf("后序名单长度 timer = %lld\n", timer);
    printf("起点 (%lld,%lld) 的盖章情况 vis[%lld][%lld] = %d\n", sx, sy, sx, sy, (int)vis[sx][sy]);
    printf("名单最后一项: (%lld,%lld)  (正确应为 (%lld,%lld))\n",
           ord[timer].x - 1, ord[timer].y - 1, sx - 1, sy - 1);
    return 0;
}
