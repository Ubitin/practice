#include<bits/stdc++.h>
using namespace std;
using ll = long long;

ll len,n,k; vector<ll>road;

bool p(ll x){
    vector<ll>d(n);
    ll res = 0;

    for(ll i = 0;i<n - 1;++i){
        d[i] = road[i + 1] - road[i];

        if(d[i] > x){
            if(d[i] % x == 0) res += (d[i]/x - 1);
            else res += floor(d[i]/x);
        }
    }

    return res <= k;
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

    cin>>len>>n>>k;
    road.resize(n);
    for(ll i = 0;i<n;++i) cin>>road[i];

    ll l = 1,r = 1e8;
    ll ans = find(l , r);

    cout<<ans;

    return 0;
}