#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const ll maxn = 1005;
ll n,k,ans;
ll a[maxn];

bool p(ll x) {
    for(ll i = 1;i<=n - 1;i++) {
        ll rem = k; ll need = x;
        ll j = i;
        for(;j <= n - 1;++j) {
            if(a[j] >= need) break;
            rem -= (need - a[j]);
            if(rem < 0) break;
            need--;  
        }
        if(rem >= 0 && (a[j] >= need || a[n] >= need)) return true;
    }
    return false;
}

ll find(ll l ,ll r) {
    ll mid = (l + r) >> 1;
    ll ans = l;
    while(l <= r) {
        if(p(mid = (l + r) >> 1)) {
            ans = mid;
            l = mid + 1;
        }
        else r = mid - 1;
    }
    return ans;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll t ; cin>>t;
    while(t--){
        cin>>n>>k;
        ll l = 0, r;
        for(ll i = 1;i<=n;++i) {
            cin>>a[i];
            l = max(l,a[i]);
        }
        r = l + k;
        ans = find(l , r);

        cout<<ans<<"\n";
    }

    return 0;
}