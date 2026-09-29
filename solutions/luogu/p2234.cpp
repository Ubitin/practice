#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const ll maxn = 32769;
const ll INF = 1e18;
struct a{
    ll l , r; ll sa,da;
};
a v[maxn];
ll ris[maxn];//第i天对应的序号
bool cmp(a x,a y){
    return x.sa < y.sa;//cmp函数里面不能用>=或者<=
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n; cin>>n; ll ans = 0;
    for(ll i = 1;i<=n;++i){
        cin>>v[i].sa;
        v[i].da = i;
    }

    sort(v + 1,v + n + 1,cmp);
    v[0] = {n + 1,1,-INF,0}; v[n + 1] = {n,0,INF,n + 1};

    for(ll i = 1;i <= n;++i){
        v[i].l = i - 1; v[i].r = i + 1;
        ris[v[i].da] = i;
    }

    for(ll j = n;j>=2;--j) {
        ll res = 0;
        ll id = ris[j];
        res = min(abs(v[id].sa - v[v[id].l].sa),abs(v[id].sa - v[v[id].r].sa));
        ans += res;
        v[v[id].l].r = v[id].r; v[v[id].r].l = v[id].l;
    }

    ans += v[ris[1]].sa;
    cout<<ans;

    return 0;
}