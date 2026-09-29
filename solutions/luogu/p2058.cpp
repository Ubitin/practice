#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const ll maxn = 2e5;
struct pas{
    ll t, nat;//到达的时间，国籍
};

queue<pas>q;//一个维护乘客的队列
ll n;//船只数量
ll cnt[maxn] = {0};//每个国家的人数
ll ans = 0;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n;
    while(n--){
        ll time,contain;//到达时间，容量
        cin>>time>>contain;
        while(contain--){//把乘客塞进队列
            pas temp; temp.t = time;
            cin>>temp.nat; q.push(temp);
            cnt[temp.nat] ++;
            if(cnt[temp.nat] == 1) ans++;//由 0 -> 1,ans++
        }

        while(!q.empty() && q.front().t <= time - 86400) {
            ll na = q.front().nat;
            if(--cnt[na] == 0) ans--;//由1 -> 0,ans--
            q.pop();
        }

        cout<<ans<<"\n";
    }

    return 0;
}