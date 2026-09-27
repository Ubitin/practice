#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const ll rif = 1e9;
const ll maxn = 2005;
struct dot{
    ll x,y;
};
ll rm[4][2] = {{0,1},{0,-1},{-1,0},{1,0}};
ll x1,y1,x2,y2;
ll ans = 0;
ll n;
ll ti[maxn][maxn];

dot wind(char c){
    if(c == 'U') return {0,1};
    else if(c == 'D') return {0,-1};
    else if(c == 'L') return {-1,0};
    else if(c == 'R') return {1,0};
    return {0,0};
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>x1>>y1>>x2>>y2>>n;
    memset(ti,-1,sizeof(ti));
    string wing; cin>>wing;
    
    queue<dot>q;
    q.push({x1,y1});

    while(!q.empty()){
        dot u = q.front(); q.pop();
        ll ux = u.x , uy = u.y;
        ll t = ti[ux][uy];
        if(t <= n){
            dot mo = wind(wing[t]);
            ux += mo.x; uy += mo.y;
        }
        dot d; ll nx,ny;
        for(ll i = 0;i<4;++i){
            nx = ux + rm[i][0]; ny = uy + rm[i][1];
            if(nx < 0 || nx >  rif || ny < 0 || ny > rif) continue;
            if(ti[nx][ny] != -1) continue;
            d = {nx,ny}; ti[nx][ny] = t + 1;
            q.push(d);
        }
    }

    cout<<ti[x2][y2];

    return 0;
}