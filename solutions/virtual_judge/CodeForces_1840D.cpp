#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const ll maxn = 1e6;
ll t,n;
vector<ll>a(maxn);//每个顾客的型号

bool p(ll u){//最大时间u是否满足“最大”这个条件
    ll num = 1;//需要的匠人的数量
    ll temp = a[0];
    for(ll i = 0;i<n;++i){
        if(a[i] <= temp + 2 * u) continue;
        else{
            num++; temp = a[i];
        }
    }
    return num <= 3;
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

    cin>>t;

    while(t--){
        cin>>n;
        
        for(ll i = 0;i<n;++i) cin>>a[i];
        sort(a.begin(),a.begin() + n);
        ll l = 0,r = 1e9;
        ll ans = find(l,r);
        cout<<ans<<"\n";
    }

    return 0;
}