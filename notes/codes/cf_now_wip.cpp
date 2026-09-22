#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const ll maxn = 1e6;
ll t,n;ll x,y,z;//三个匠人的型号
ll a[maxn];//每个顾客的型号

bool p(ll u){//最大时间u是否满足“最大”这个条件
    int ok = 1;

    for(ll i = 0;i<n;++i){
        if(abs(x - a[i]) <= u || abs(y - a[i]) <= u || abs(z - a[i]) <= u) continue;
        else ok = 0;
    }

    return ok;
}

ll find(ll l,ll r){
    ll mid = (l + r) >> 1;
    ll ans = 0;

    while(l <= r){
        if(p(mid = (l + r) >> 1)){
            ans = mid;
            r = mid - 1;
        }
        l = mid + 1;
    }

    return ans;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>t;

    while(t--){
        cin>>n;
    }
}