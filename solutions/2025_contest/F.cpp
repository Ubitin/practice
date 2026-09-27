#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const ll maxn = 1e5;
ll x1,dy1,x2,y2; ll t;//周期
ll wx[maxn]; ll wy[maxn];//记录一个周期内风的方向（前缀和）
ll ux,uy;//一个周期内风的方向的前缀和

bool p(ll k){//判断在至少k天时能不能到达
    ll dx = (k / t) * ux + wx[k % t];
    ll dy = (k / t) * uy + wy[k % t];

    ll dis = abs(x1 + dx - x2) + abs(dy1 + dy - y2);

    return dis <= k;
}

ll find(ll l,ll r){
    ll mid = (l + r) >> 1;
    ll ans = -1;
    while(l <= r){
        if(p(mid = (l + r) >> 1)){
            ans = mid;
            r = mid - 1;
        }
        else l = mid + 1;
    }
    return ans;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>x1>>dy1>>x2>>y2;
    cin>>t;
    wx[0] = 0,wy[0] = 0;
    for(ll i = 1;i<=t;++i){
        char c; cin>>c;
        if(c == 'U') wy[i] = wy[i - 1] + 1;
        else if(c == 'D') wy[i] = wy[i - 1] - 1;
        else if(c == 'L') wx[i] = wx[i - 1] - 1;
        else if(c == 'R') wx[i] = wx[i - 1] + 1;
    }
    ux = wx[t] , uy = wy[t];
    ll l = 1, r = 1e9;
    ll ans = find(l , r);
    cout<<ans;

    return 0;
}