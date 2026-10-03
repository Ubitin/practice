#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const ll maxn = 1e6 + 5;

ll l[maxn]; ll r[maxn];

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n; cin>>n;
    ll ans = 0;
    vector<pair<ll,ll>>node(n + 1);
    for(ll i = 1;i<=n;++i) {
        cin>>l[i]>>r[i];
    }

    node.push_back({1,1});//序号，深度
    while(!node.empty()) {
        auto [u,v] = node.back(); node.pop_back();

        if(l[u] != 0) {
            pair<ll,ll>tmp = {l[u],v + 1};
            node.push_back(tmp);
            ans = max(ans,v + 1);
        }
        if(r[u] != 0) {
            pair<ll,ll>tmp = {r[u],v + 1};
            node.push_back(tmp);
            ans = max(ans,v + 1);
        }
    }

    cout<<ans;

    return 0;
}