#include<bits/stdc++.h>
using namespace std;
using ll = long long;

ll n,m,k;
struct dot{
    ll x,y;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>m>>n>>k;
    ll cnt[m + 1][n + 1];//达到该状态所需的步数
    ll b[m + 1][n + 1];//该状态是否被访问过
    memset(b,-1,sizeof(b));//-1表示未被访问过

    dot d = {0,0};
    queue<dot>q;
    q.push(d); b[0][0] = 0; cnt[0][0] = 0;
    ll ans = 1e18;

    while(!q.empty()) {
        dot u = q.front(); q.pop();
        ll ux = u.x; ll uy = u.y;
        ll cn = cnt[ux][uy];
        ll nx,ny;
        for(ll i = 0;i<6;++i) {
            if(i == 0) {//小杯清空
                nx = 0 ,ny = uy;
                if(b[nx][ny] != -1) continue;
                else {
                    cnt[nx][ny] = cn + 1;
                    b[nx][ny] = 0;
                    q.push({nx,ny});
                }
                if(nx == k || ny == k) {
                    ans = min(cnt[nx][ny],ans);
                    break;
                }
            }
            else if(i == 1) {//清空大杯
                nx = ux ,ny = 0;\
                if(b[nx][ny] != -1) continue;
                else {
                    cnt[nx][ny] = cn + 1;
                    b[nx][ny] = 0;
                    q.push({nx,ny});
                }
                if(nx == k || ny == k) {
                    ans = min(cnt[nx][ny],ans);
                    break;
                }
            }
            else if(i == 2) {//小杯倒入大杯
                ll t = min(ux,n - uy);
                nx = ux - t , ny = uy + t;
                if(b[nx][ny] != -1) continue;
                else {
                    cnt[nx][ny] = cn + 1;
                    b[nx][ny] = 0;
                    q.push({nx,ny});
                }
                if(nx == k || ny == k) {
                    ans = min(cnt[nx][ny],ans);
                    break;
                }
            }
            else if(i == 3) {//大杯倒入小杯
                ll t = min(m - ux,uy);
                nx = ux + t; ny = uy - t;
                if(b[nx][ny] != -1) continue;
                else {
                    cnt[nx][ny] = cn + 1;
                    b[nx][ny] = 0;
                    q.push({nx,ny});
                }
                if(nx == k || ny == k) {
                    ans = min(cnt[nx][ny],ans);
                    break;
                }
            }
            else if(i == 4) {//小杯装满
                nx = m,ny = uy;
                if(b[nx][ny] != -1) continue;
                else {
                    cnt[nx][ny] = cn + 1;
                    b[nx][ny] = 0;
                    q.push({nx,ny});
                }
                if(nx == k || ny == k) {
                    ans = min(cnt[nx][ny],ans);
                    break;
                }
            }
            else if(i == 5) {//大杯装满
                nx = ux; ny = n;
                if(b[nx][ny] != -1) continue;
                else {
                    cnt[nx][ny] = cn + 1;
                    b[nx][ny] = 0;
                    q.push({nx,ny});
                }
                if(nx == k || ny == k) {
                    ans = min(cnt[nx][ny] , ans);
                    break;
                }
            }
        }
    }

    cout<<ans;

    return 0;
}