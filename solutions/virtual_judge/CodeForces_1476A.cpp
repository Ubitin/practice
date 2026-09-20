#include<bits/stdc++.h>
using namespace std;
using ll = long long;
ll n,k;

bool p(ll x){
    ll sum = n - 1 + x;
    ll temp = sum % k;
    if(temp == 0) return true;

    else{
        ll res = k - temp;
        if(!(res % (n - 1))){
            res /= (n - 1);
        }
        else{
            res = res / (n - 1) + 1;
        }

        return x >= (res + 1);
    }
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

    ll t; cin>>t;
    while(t--){
        cin>>n>>k;
        if(n == 1) cout<<k<<"\n";
        else{
            ll ans = 0;
            ll l = 1,r = 2e9;
            ans = find(l,r);
            cout<<ans<<"\n";
        }
    }

    return 0;
}