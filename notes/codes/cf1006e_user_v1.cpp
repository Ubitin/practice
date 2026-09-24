#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const ll maxn = 200005;

vector<ll> ch[maxn];//子节点//问题：为什么不能写成：vector<vector<ll>>ch;
ll pos[maxn];//i对应名单第几个
ll ord[maxn];//名单上第i个是谁
ll cnt[maxn];//i的手下有多少个 
ll timer_ = 0;

void dfs(ll u){
    pos[u] = ++timer_;
    ord[timer_] = u;
    cnt[u] = 1;

    for(auto v : ch[u]) {
        dfs(v);
        cnt[u] += cnt[v];
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n,m; cin>>n>>m;
    
    for(ll i = 2;i<=n;++i) {
        ll x; cin>>x;
        ch[x].push_back(i);
    }

    dfs(1);
    while(m--){
        ll u,k ; cin>>u>>k;
        
        cout<<(k > cnt[u] ? -1 : ord[pos[u] + k - 1])<<"\n";
    }
    return 0;
}
