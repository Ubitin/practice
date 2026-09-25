// dfs_param_index_demo.cpp —— 第二种写法：保留你熟悉的"一个参数 u"的风格
// 把格子编号成一维：u = x * m + y（x、y 都用 0 下标），反解 x = u / m, y = u % m
// 输出格式与 dfs_param_demo.cpp 完全一致，方便逐字节对比：两种写法名单必须一模一样。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll maxn = 502;
const ll maxe = maxn * maxn;

ll n, m, k;
bool wall[maxn][maxn];
bool vis[maxn][maxn];
ll timer = 0;
struct dot { ll x, y; };
dot ord[maxe];

void dfs(ll u) {                 // ← 唯一的参数：格子编号 u
    ll x = u / m, y = u % m;     // 把编号还原成坐标
    vis[x][y] = true;
    const ll dx[4] = {0, 0, 1, -1}, dy[4] = {1, -1, 0, 0};
    for (ll d = 0; d < 4; ++d) {
        ll nx = x + dx[d], ny = y + dy[d];
        if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;   // 0 下标，越界就是 >= n / >= m
        if (wall[nx][ny] || vis[nx][ny]) continue;
        dfs(nx * m + ny);        // 邻居也要先编号再递归
    }
    ord[++timer] = {x, y};
}

int main() {
    if (scanf("%lld %lld %lld", &n, &m, &k) != 3) return 1;
    ll sx = -1, sy = -1, blank = 0;
    for (ll i = 0; i < n; ++i) {
        static char buf[maxn];
        scanf("%s", buf);
        for (ll j = 0; j < m; ++j) {
            wall[i][j] = (buf[j] == '#');
            if (!wall[i][j]) { ++blank; if (sx < 0) { sx = i; sy = j; } }
        }
    }
    dfs(sx * m + sy);

    printf("空格总数 blank = %lld\n", blank);
    printf("后序名单长度 timer = %lld  (应等于 blank)\n", timer);
    printf("起点 (%lld,%lld) 在名单里的位置 = ", sx, sy);
    for (ll i = 1; i <= timer; ++i) if (ord[i].x == sx && ord[i].y == sy) { printf("%lld\n", i); break; }
    ll show = timer < 12 ? timer : 12;
    printf("名单前 %lld 项: ", show);
    for (ll i = 1; i <= show; ++i) printf("(%lld,%lld)%s", ord[i].x, ord[i].y, i == show ? "\n" : " ");
    printf("名单最后一项: (%lld,%lld)\n", ord[timer].x, ord[timer].y);
    return 0;
}
