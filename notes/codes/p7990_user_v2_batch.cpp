// 你【当前版本】test_\01.cpp 的批量对拍专用副本
// ⚠️ 逻辑与原码完全一致（max_element 也没换成前缀最大值，保留原样以测真实性能）；
//    只加了 while(cin>>n) 与每组重置，便于一次喂入多个用例。
// ⚠️ 你的原文件没有被修改。
#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const ll maxn = 1e6;
ll n,q;
ll k,p,x;
ll a[maxn],t[maxn];
ll res = 1e9,last = 0;
vector<ll>mon;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    while(cin>>n){
    res = 1e9; last = 0;
    for(ll i = 1;i <= n;++i) {
        cin>>a[i]; t[i] = 0;
        res = min(res , a[i]);
    }

    cin>>q;
    mon.assign(q + 1, 0);
    for(ll i = 1;i <= q;++i){
        cin>>k;
        if(k == 1){
            cin>>p>>x;
            a[p] = x;
            t[p] = i;
        }
        else if(k == 2){
            cin>>x;
            mon[i] = x;
        }
    }


    for(ll i = 1;i <= n;++i){
        ll temp = *max_element(mon.begin() + t[i],mon.end());
        cout<<max(a[i] , temp)<<" ";
    }
    cout<<"\n";
    }
    return 0;
}
