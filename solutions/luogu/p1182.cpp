#include<bits/stdc++.h>
using namespace std;
using ll = long long;

ll n,m; vector<ll>a;

bool p(ll x){
    ll cnt = 1; ll cur = 0;

    for(ll i = 0;i<n;++i){
        if(cur + a[i] <= x) cur += a[i];
        else{
            cur = a[i]; cnt++;
        }
    }

    return cnt <= m;
}

ll find(ll l,ll r){
    ll mid = (l + r) >> 1;
    ll ans = 0;

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

    cin>>n>>m; a.resize(n);
    a[0] = 0;
    
    ll l = 0,r = 0;
    
    for(ll i = 0;i<n;++i){
        cin>>a[i];
        l = max(l,a[i]);
        r += a[i];
    }
    ll ans = find(l , r);

    cout<<ans;

    return 0;
}