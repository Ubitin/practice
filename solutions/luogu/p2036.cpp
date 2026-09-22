//思路：用DFS去遍历所有合法的方案，同时用一个变量ans滚动维护，每个取min
//也就是说在k > n之后不需要维护答案，直接return,因为在迭代的每一步我们都取了min的，保证了ans一定是最小的
#include<bits/stdc++.h>
using namespace std;
using ll = long long;
ll n,ans;
struct food{
    ll s,b;
};
bool t[12];
ll ss = 1,sb = 0;//用于记录酸度的乘积，苦度的和
food fo[12];

void dfs(ll k){
    if(k > n){
        return;
    }

    for(ll i = 0;i<n;++i){
        if(t[i] == 0){
            ss *= fo[i].s; sb += fo[i].b;
            ll res = abs(ss - sb);
            t[i] = 1;
            ans = min(res , ans);
            dfs(k + 1);
            ss /= fo[i].s; sb -= fo[i].b;
            t[i] = 0;
        }
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n;
    for(ll i = 0;i<n;++i) cin>>fo[i].s>>fo[i].b;

    ans = 1e9;
    dfs(1);

    cout<<ans;
    return 0;
}