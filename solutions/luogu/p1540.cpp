#include<bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll m,n; cin>>m>>n;
    queue<ll>mem; ll dis[1005];
    memset(dis,-1,sizeof(dis));
    ll tot = 0;
    ll ans = 0;

    while(n--){
        ll x = 0; cin>>x;
        if(dis[x] != -1) continue;
        else{
            ans++;
            dis[x] = 0;
            if(tot < m) {
                tot ++;
                mem.push(x);
            }
            else if(tot == m){
                dis[mem.front()] = -1;
                mem.pop();
                mem.push(x);
            }
        }
    }
    cout<<ans;
    return 0;
}