#include<bits/stdc++.h>
using namespace std;
using ll = long long;

ll n,t;
ll ans = 0;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>t;
    while(t--){
        ans = 0;//注意初始化
        cin>>n;
        for(ll i = 3;i<n;++i){
            if(i%3 && i%5) continue;

            ans += i;
        }
        cout<<ans<<"\n";
    }
    return 0;
}