#include<bits/stdc++.h>
using namespace std;
using ll = long long;
ll ans = 0; ll n;
ll a[100];
ll l[100],x[100],y[100];

void dfs(ll k){
    if(k > n){
        ans ++;
        if(ans <= 3){//注意题干，说的是输出前三个答案
            for(ll i = 1;i<=n;++i) cout<<a[i]<<" ";
            cout<<"\n";
        }
        return;
    }

    else{
        for(ll i = 1;i<=n;++i){
            ll lx = k - i + 15,ly = k + i;//k - i必须 + 15，因为k - i可能为负，但是数组不能记录负下标，所以要加上15
            if(l[i] == 0 && x[lx] == 0 && y[ly] == 0){
                l[i] = 1,x[lx] = 1,y[ly] = 1;
                a[k] = i;
                dfs(k + 1);
                l[i] = 0,x[lx] = 0,y[ly] = 0;
            }
        }
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n;
    dfs(1);
    
    cout<<ans<<"\n";

    return 0;
}