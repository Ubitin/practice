#include<bits/stdc++.h>
using namespace std;
using ll = long long;
ll n; ll cnt = 0; ll res = 0;

void dfs(ll k){
    if(k > 4){
        if(res == n) cnt++;
        return;
    }

    for(int i = 0;i <= 9;++i){
        long long temp = res + i;
        if(temp > n) continue;
        res += i;
        dfs(k + 1);
        res -= i;
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n;
    dfs(1);
    cout<<cnt;

    return 0;
}