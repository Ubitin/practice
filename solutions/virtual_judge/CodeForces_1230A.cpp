#include<bits/stdc++.h>
using namespace std;
using ll = long long;
ll a[5],t[5];
bool ok = 0;
ll res = 0,sum = 0;

void dfs(int k){
    if(k > 4){
        return;
    }

    
    for(int i = 0;i<4;++i){
        if(t[i] != -1) continue;

        else
        res += a[i]; t[i] = 0;
        if(res == sum / 2){
            ok = 1;
        }
        dfs(k + 1);
        res -= a[i]; t[i] = -1;
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    for(int i = 0;i<4;++i){cin>>a[i]; sum += a[i];};
    memset(t,-1,sizeof(t));//一定不要忘了初始化

    if(sum % 2){
        cout<<"NO\n";
        return 0;
    }
    else{
        dfs(1);
        cout<<(ok ? "YES\n" : "NO\n");
    }

    return 0;
}