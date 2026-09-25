// ===== 只改一行的修正版（用来验证"是不是只有这一处问题"）=====
// 与用户 test_\01.cpp 逐字节相同，唯一区别在第 44 行：
//   原：  temp = '#' ? wall[j][i] = 1 : wall[j][i] = 0;
//   改：  wall[j][i] = (temp == '#');
#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const ll maxn = 502;

struct dot{
    ll x,y;
};

ll rm[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
ll n,m,k; //迷宫的深度，宽度，欲添加的墙的数目
bool wall[maxn][maxn];//记录该坐标是否是墙
bool blank[maxn][maxn];//记录该空地是否被访问过
ll timer = 0;
dot cnt[maxn * maxn];//后序名单，第i个是哪个点
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
    ll tx = 1, ty = 1;              // ★ 顺手给了初值（原版没给，只是为消警告）
    for(ll i = 1;i<=n;++i){
        for(ll j = 1;j<=m;++j){
            cin>>temp; c[j][i] = temp;
            wall[j][i] = (temp == '#');     // ★★ 唯一的一行实质修改 ★★
            blank[j][i] = 0;
        }
    }

    for(ll i = 1;i<=n;++i){
        for(ll j = 1;j <= m;++j){
            if(c[j][i] == '.'){
                tx = j; ty = i;
                break;
            }
        }
    }

    dfs(tx,ty);

    for(ll i = 1;i<=k;++i){
        ll x = cnt[i].x; ll y = cnt[i].y;
        c[x][y] = 'X';
    }

    for(ll i = 1;i<=n;++i){
        for(ll j = 1;j<=m;++j){
            cout<<c[j][i];
        }
        cout<<"\n";
    }

    return 0;
}
