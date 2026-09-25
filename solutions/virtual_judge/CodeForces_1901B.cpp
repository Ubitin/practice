#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const ll maxn = 1e6;
ll n,t;
ll a[maxn];

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>t;
    while(t--){
        cin>>n;
        for(ll i = 0;i<n;++i){
            cin>>a[i];
        }

        ll ans = 0;
        for(ll i = 1;i<n;++i){
            if(a[i] > a[i - 1]){
                ans += a[i] - a[i - 1];
            }
        }
        ans += a[0];

        cout<<ans - 1<<"\n";
    }
    return 0;
}