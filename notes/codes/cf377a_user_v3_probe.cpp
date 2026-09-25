// ===== 探针版：用户 v3 代码 + 诊断输出（用来把"问题出在哪里"量化做实）=====
// 与原码唯一的区别：读完网格后打印 wall 里 true 的个数；dfs 后打印 timer 和前几个名单项
#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const ll maxn = 502;

struct dot{
    ll x,y;
};

ll rm[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
ll n,m,k;
bool wall[maxn][maxn];
bool blank[maxn][maxn];
ll timer = 0;
dot cnt[maxn * maxn];
char c[maxn][maxn];


void dfs(ll x, ll y){
    blank[x][y] = true;
    for(ll i = 0;i<4;++i){
        ll nx = x + rm[i][0]; ll ny = y + rm[i][1];
        if(nx < 1 || nx > m || ny < 1 || ny > n) continue;
        if(!wall[nx][ny]){
            if(blank[nx][ny]) continue;
        }
        if(wall[nx][ny]) continue;
        dfs(nx,ny);
    }
    cnt[++timer] = {x , y};
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n>>m>>k;
    char temp;
    ll tx,ty;
    for(ll i = 1;i<=n;++i){
        for(ll j = 1;j<=m;++j){
            cin>>temp; c[j][i] = temp;
            temp = '#' ? wall[j][i] = 1 : wall[j][i] = 0;
            blank[j][i] = 0;
        }
    }

    // ---- 探针 1：wall 数组里 true 的个数（真正的墙有多少个）----
    ll wallcnt = 0, trueWall = 0;
    for(ll i = 1;i<=n;++i) for(ll j = 1;j<=m;++j){
        if(wall[j][i]) ++wallcnt;
        if(c[j][i] == '#') ++trueWall;
    }
    fprintf(stderr, "[探针] 网格里真实的 '#' = %lld 个；wall[][] 里被标成 1 的 = %lld 个\n", trueWall, wallcnt);

    for(ll i = 1;i<=n;++i){
        for(ll j = 1;j <= m;++j){
            if(c[j][i] == '.'){
                tx = j; ty = i;
                break;
            }
        }
    }
    fprintf(stderr, "[探针] 起点搜到的是 (x=%lld, y=%lld)，字符 = '%c'\n", tx, ty, c[tx][ty]);

    dfs(tx,ty);

    // ---- 探针 2：dfs 到底走了几个格子 ----
    ll blanks = 0;
    for(ll i = 1;i<=n;++i) for(ll j = 1;j<=m;++j) if(c[j][i] == '.') ++blanks;
    fprintf(stderr, "[探针] 网格里 '.' = %lld 个；dfs 后序名单长度 timer = %lld\n", blanks, timer);
    fprintf(stderr, "[探针] 名单前几项: ");
    for(ll i = 1;i <= min(timer, (ll)5); ++i)
        fprintf(stderr, "(%lld,%lld) ", cnt[i].x, cnt[i].y);
    fprintf(stderr, "\n");

    for(ll i = 1;i<=k;++i){
        ll x = cnt[i].x; ll y = cnt[i].y;
        c[x][y] = 'X';
    }

    // ---- 探针 3：到底往哪几个坐标写了 'X' ----
    fprintf(stderr, "[探针] 写 'X' 的坐标: ");
    for(ll i = 1;i<=k;++i) fprintf(stderr, "(%lld,%lld) ", cnt[i].x, cnt[i].y);
    fprintf(stderr, "\n");

    for(ll i = 1;i<=n;++i){
        for(ll j = 1;j<=m;++j){
            cout<<c[j][i];
        }
        cout<<"\n";
    }

    return 0;
}
