// dfs_param_demo.cpp —— 只演示一件事：
// CF377A 这类"网格题"的 dfs，参数到底该传什么？
// 用法: dfs_param_demo.exe < in.txt   （输入格式同 CF377A: n m k 然后 n 行网格）
// 结论：传"当前格子的坐标 (x, y)"两个整数，不传一维下标 u。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll maxn = 502;          // 网格最大 500×500，加点余量
const ll maxe = maxn * maxn;  // 空格最多 n*m 个 → 后序名单要开这么大！

ll n, m, k;
bool wall[maxn][maxn];        // true = 墙
bool vis[maxn][maxn];         // true = 走过了
ll timer = 0;                 // 后序名单已经装了几个
struct dot { ll x, y; };
dot ord[maxe];                // 后序名单

const ll dx[4] = {0, 0, 1, -1};   // 右下左上，顺序无所谓
const ll dy[4] = {1, -1, 0, 0};

// ↓↓↓ 参数就是这两个：当前格子的横纵坐标 ↓↓↓
void dfs(ll x, ll y) {
    vis[x][y] = true;                     // 进门前先盖章，防止回头
    for (ll d = 0; d < 4; ++d) {
        ll nx = x + dx[d], ny = y + dy[d];
        if (nx < 1 || nx > n || ny < 1 || ny > m) continue;  // 出界
        if (wall[nx][ny] || vis[nx][ny]) continue;           // 是墙 / 走过
        dfs(nx, ny);                                         // 换一个新坐标递归
    }
    ord[++timer] = {x, y};                // 四个方向全走完 → 出门时记一笔（后序）
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
    dfs(sx, sy);                          // 从第一个空格出发

    printf("空格总数 blank = %lld\n", blank);
    printf("后序名单长度 timer = %lld  (应等于 blank)\n", timer);
    printf("起点 (%lld,%lld) 在名单里的位置 = ", sx - 1, sy - 1);
    for (ll i = 1; i <= timer; ++i) if (ord[i].x == sx && ord[i].y == sy) { printf("%lld\n", i); break; }
    ll show = timer < 12 ? timer : 12;
    printf("名单前 %lld 项: ", show);
    for (ll i = 1; i <= show; ++i) printf("(%lld,%lld)%s", ord[i].x - 1, ord[i].y - 1, i == show ? "\n" : " ");
    printf("名单最后一项: (%lld,%lld)\n", ord[timer].x - 1, ord[timer].y - 1);
    return 0;
}
