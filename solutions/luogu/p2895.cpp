#include<bits/stdc++.h>
using namespace std;
using ll = long long;

#ifndef grid_f
#define grid_f 310//限定范围
#endif
const ll N = grid_f;
const ll INF = (ll)1e18; 

ll m = 0;
ll rm[4][2] = {{1,0},{0,1},{-1,0},{0,-1}};//移动的四个方向
ll b[310][310];//每个格子发生爆炸的时间
ll ti[310][310];//记录走到每个格子所需的最短时间

struct dot{//用于记录坐标的结构体
    ll x,y;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    memset(ti,-1,sizeof(ti));//-1表示未访问过
    for(ll i = 0;i<310;++i){//每个格子爆炸的时间初始为INF，即超出上限，视为不会爆炸
        for(ll j = 0;j<310;++j){
            b[i][j] = (ll)INF;
        }
    }
    cin>>m;
    for(ll i = 0;i<m;++i){//记录每个格子陨石落下的时间
        ll x,y,t; cin>>x>>y>>t;
        auto mark = [&](ll a,ll c){//越界保护，超出范围的不写入
            if(a >= 0 && a < N && c >= 0 && c < N && t < b[a][c]){b[a][c] = (ll)t;}
        };
        mark(x,y);
        for(ll j = 0;j<4;++j){
            mark(x + rm[j][0] , y + rm[j][1]);
        }
    }

    queue<dot>q;//广度优先搜索的队列
    dot o = {0,0}; q.push(o);
    ti[0][0] = 0;

    while(!q.empty()){
        dot u = q.front(); q.pop();
        ll ux = u.x , uy = u.y;
        ll t = ti[ux][uy];
        for(ll i = 0;i<4;++i){
            ll nx = ux + rm[i][0] , ny = uy + rm[i][1];
            if(nx < 0 || nx >= N || ny < 0 || ny >= N || (t + 1) >= b[nx][ny] || ti[nx][ny] != -1) continue;

            ti[nx][ny] = t + 1;
            dot d = {nx , ny};
            q.push(d);
        }
    }

    ll ans = INF;
    for(ll i = 0;i<310;++i){
        for(ll j = 0;j<310;++j){
            if(b[i][j] > 1000 && ti[i][j] != -1){
                ans = min(ans,ti[i][j]);
            }
        }
    }

    if(ans > 1000) cout<<-1;
    else cout<<ans;

    return 0;
}