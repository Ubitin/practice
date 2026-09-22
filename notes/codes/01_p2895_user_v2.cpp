#include<bits/stdc++.h>
using namespace std;
using ll = long long;
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
    for(ll i = 0;i<310;++i){//每个格子爆炸的时间初始为1002，即超出上限，视为不会爆炸
        for(ll j = 0;j<310;++j){
            b[i][j] = (ll)INF;
        }
    }
    cin>>m;
    for(ll i = 0;i<m;++i){//记录每个格子陨石落下的时间
        ll x,y,t; cin>>x>>y>>t;
        b[x][y] = min(t,b[x][y]);//取最早的时间为爆炸的时间
        for(ll i = 0;i<4;++i){
            ll xt = x + rm[i][0] , yt = y + rm[i][1];
            if(xt >= 0 && yt >= 0){
                b[xt][yt] = min(t,b[xt][yt]);
            }
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
            if(nx < 0 || ny < 0 || (t + 1) >= b[nx][ny] || ti[nx][ny] != -1) continue;

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