#include<bits/stdc++.h>
using namespace std;
using ll = long long;

ll s[5],a[22];
ll ans = 0,maxdeep,sum;
ll maxtime,nowtime;

void dfs(ll k){
    if(k > maxdeep){
        maxtime = max(maxtime,nowtime);
        return;
    }

    if((nowtime + a[k]) <= (sum / 2)){
        nowtime += a[k];              
        dfs(k + 1);
        nowtime -= a[k];
    }
    dfs(k + 1);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    memset(s,0,sizeof(s)); memset(a,0,sizeof(a));
    for(int i = 0;i<4;++i) cin>>s[i];

    for(ll i = 0;i<4;++i){
        maxtime = 0; nowtime = 0;
        maxdeep = s[i]; sum = 0;

        for(ll j = 1;j<=s[i];++j){
            cin>>a[j]; sum += a[j];
        }
        dfs(1);
        ans += (sum - maxtime);
    }

    cout<<ans;

    return 0;
}