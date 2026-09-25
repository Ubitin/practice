#include<bits/stdc++.h>
using namespace std;
using ll = long long;

struct bus{
    ll st,d,id;//分别记录公交的初始时间，间隔，序号
    ll time;//第一次超过秋绘到站时刻的时间
};
const ll maxn = 102;
ll n,t; bus a[maxn];

bool cmp(bus b1,bus b2){
    if(b1.time != b2.time) return b1.time < b2.time;
    else if(b1.time == b2.time) return b1.id < b2.id;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n>>t;

    for(ll i = 0;i<n;++i) {
        cin>>a[i].st>>a[i].d;
        a[i].id = i + 1;
        if(a[i].st >= t) a[i].time = a[i].st;
        else if(a[i].st < t){
            ll temp = 0;
            if((t - a[i].st) % a[i].d) {
                temp = (t - a[i].st) / a[i].d + 1;
                a[i].time = a[i].st + temp * a[i].d; 
            }
            else a[i].time = t;
        }
    }

    sort(a,a + n,cmp);
    
    cout<<a[0].id;

    return 0;
}