// ===== 参考修正版（在你 v4 基础上只动四处；★ 标出）=====
// 说明：这是【新建的对照文件】，你原来的 test_\01.cpp 我没动。
// 改动清单：
//   ★1 char c[maxn][maxn] 统一成【行在前】c[行][列]（原来是 c[列][行] 转置存），读入/打印/判界一起改
//   ★2 起点搜索：找到第一个就【跳两层】（原版 break 只跳内层，起点变成"最后一个含 '.' 的行里最靠左的空格"）
//   ★3 tx,ty 给初值（消掉 GCC 的 may-be-uninitialized 警告）
//   ★4 dfs 里两段绕的 if 合成一行：if (wall || blank) continue;
// 用法不变：读 n m k 与网格，输出改完的网格。本地大数据请加：-Wl,--stack,268435456
#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const ll maxn = 505;                 // 500 + 余量
const ll maxe = 250005;              // 空格最多 n*m = 250000

struct dot{
    ll x,y;
};

ll rm[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
ll n,m,k;                            // n 行、m 列
bool wall[maxn][maxn];               // true = 墙
bool blank[maxn][maxn];              // true = 走过
ll timer = 0;
dot cnt[maxe];                       // 后序名单（按 n*m 开）
char c[maxn][maxn];                  // ★1 行在前：c[行][列]

void dfs(ll x, ll y){                // x = 行号、y = 列号（都 1-based）
    blank[x][y] = true;
    for(ll i = 0;i<4;++i){
        ll nx = x + rm[i][0], ny = y + rm[i][1];
        if(nx < 1 || nx > n || ny < 1 || ny > m) continue;   // ★1 判界跟着换：行用 n、列用 m
        if(wall[nx][ny] || blank[nx][ny]) continue;          // ★4
        dfs(nx,ny);
    }
    cnt[++timer] = {x, y};           // 后序：四方向看完才记账
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> k;
    for(ll i = 1;i<=n;++i){
        for(ll j = 1;j<=m;++j){
            cin >> c[i][j];                       // ★1 c[行][列]
            wall[i][j] = (c[i][j] == '#');        // （`=` 与 `==` 别再写混）
            blank[i][j] = false;
        }
    }

    ll tx = 1, ty = 1;                            // ★3
    bool found = false;
    for(ll i = 1; i <= n && !found; ++i){         // ★2 标志位跳两层
        for(ll j = 1; j <= m; ++j){
            if(c[i][j] == '.'){
                tx = i; ty = j; found = true; break;   // 注意顺序：先 tx=行 i、再 ty=列 j
            }
        }
    }

    dfs(tx, ty);

    for(ll i = 1;i<=k;++i) c[cnt[i].x][cnt[i].y] = 'X';

    for(ll i = 1;i<=n;++i){
        for(ll j = 1;j<=m;++j) cout << c[i][j];
        cout << "\n";
    }
    return 0;
}
