#include<bits/stdc++.h>
using namespace std;
using ll = long long;

ll n,k; vector<ll>score;

bool p(ll x){
    vector<ll>prev(n + 1);

    prev[0] = 0;
    for(ll i = 1;i<=n;++i){
        prev[i] = prev[i - 1] + (score[i] >= x ? 1 : -1);
    }

    bool res = 0;
    
    priority_queue<ll,vector<ll>,greater<ll>>dp;
    dp.push(prev[0]);
    for(ll j = k;j<=n;++j){
        dp.push(prev[j - k]);
        if(prev[j] > dp.top()) res = 1;
    }

    return res;
}

ll find(ll l,ll r){
    ll mid = (l + r) >> 1;
    ll ans = 0;
    while(l <= r){
        if(p(mid = (l + r) >> 1)){
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

    cin>>n>>k;
    score.resize(n + 1);
    score[0] = 0;
    for(ll i = 1;i<=n;++i) cin>>score[i];

    ll l = 0,r = 1e6;

    ll ans = find(l,r);

    cout<<ans;

    return 0;
}