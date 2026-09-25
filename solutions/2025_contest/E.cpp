#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const ll maxn = 2002;
ll h,w;
ll up[maxn][maxn];
ll down[maxn][maxn];
ll l[maxn][maxn];
ll r[maxn][maxn];
char c[maxn][maxn];

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>h>>w;
    for(ll i = 1;i<=h;++i){
        for(ll j = 1;j<=w;++j){
            cin>>c[j][i];}
    }

    for(ll i = 1;i<=w;++i){
        up[i][0] = 0;
        for(ll j = 1;j<=h;++j){
            up[i][j] = (c[i][j] == '#' ? 0 : up[i][j - 1] + 1);
        }
    }

    for(ll i = 1;i<=w;++i){
        down[i][h + 1] = 0;
        for(ll j = h;j >= 1;--j){
            down[i][j] = (c[i][j] == '#' ? 0 : down[i][j + 1] + 1);
        }
    }

    for(ll i = 1;i<=h;++i){
        l[0][i] = 0;
        for(ll j = 1;j<=w;++j){
            l[j][i] = (c[j][i] == '#' ? 0 : l[j - 1][i] + 1);
        }
    }

    for(ll i = 1;i<=h;++i){
        r[w + 1][i] = 0;
        for(ll j = w;j >=1;--j){
            r[j][i] = (c[j][i] == '#' ? 0 : r[j + 1][i] + 1);
        }
    }

    ll ans = 0;
    for(ll i = 1;i<=h;++i){
        for(ll j = 1;j<=w;++j){
            ans = max(ans,up[j][i] + down[j][i] + l[j][i] + r[j][i] - 3);
        }
    }

    cout<<ans;

    return 0;
}