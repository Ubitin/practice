#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const ll maxn = 105;
ll n , ans = 1e8;

struct node{
    ll l,r,value,fa;
}a[maxn];
ll vis[maxn];

ll cal(ll x,ll d) {
    if(!x || vis[x]) return 0;
    
    vis[x] = 1;
    return cal(a[x].l,d + 1) + cal(a[x].r,d + 1) + cal(a[x].fa,d + 1) + a[x].value * d;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);


    cin>>n;
    a[1].fa = 0;
    for(ll i = 1;i<=n;++i) {
        cin>>a[i].value>>a[i].l>>a[i].r;
        a[a[i].l].fa = i; a[a[i].r].fa = i;
    }

    for(ll i = 1;i<=n;++i) {
        memset(vis,0,sizeof(vis));
        ans = min(ans , cal(i,0));
    }

    cout<<ans;

    return 0;
}