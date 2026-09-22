#include<bits/stdc++.h>
using namespace std;
using ll = long long;

ll m,p = 0;
ll t,v;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>m;

    for(ll i = 0;i<m;++i){
        cin>>t; cin>>v;
        if(t == 1){
            p += (1LL << v);
        }
        else if(t == 2){
            if((v & p) == v) cout<<"YES\n";
            else if((v & p) != v) cout<<"NO\n";
        }
    }

    return 0;
}