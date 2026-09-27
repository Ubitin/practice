// 你的原码 test_\01.cpp 的【批量对拍专用副本】——逻辑一字不改，只把主体包进 while(cin>>n) 以支持读多个用例
// ⚠️ 你的原文件 test_\01.cpp 没有被修改；这只是为了对拍而做的包装副本。
#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const ll maxn = 1e6;
ll n,q;
ll k,p,x;
ll timer1 = 0,timer2 = 0;
ll a[maxn],t[maxn];
ll res = 1e9,last = 0;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    while(cin>>n){                                  // ← 仅为对拍加的循环（原码此处是单次 cin>>n）
    timer1 = 0; timer2 = 0; res = 1e9; last = 0;    // ← 仅为对拍加的重置（原码是全局初值）
    for(ll i = 1;i <= n;++i) {
        cin>>a[i]; t[i] = 0;
        res = min(res , a[i]);
    }

    cin>>q;
    for(ll i = 1;i <= q;++i){
        cin>>k;
        if(k == 1){
            cin>>p>>x;
            a[p] = x;
            t[p] = i;
        }
        else if(k == 2){
            cin>>x;
            if(x >= res) {
                res = x; timer1 = i;
            }
            timer2 = i;
            last = x;
        }
    }


    for(ll i = 1;i <= n;++i){
        if(t[i] < timer1) cout<<max(res , a[i] )<<" ";
        else if(t[i] > timer1 && t[i] < timer2) {
            cout<<max(last , a[i])<<" ";
        }
        else if(t[i] > timer2) cout<<a[i]<<" ";
    }
    cout<<"\n";                                     // ← 仅为批量对拍加的行尾换行
    }                                               // ← while 结束
    return 0;
}
