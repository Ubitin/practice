#include<bits/stdc++.h>
using namespace std;
using ll = long long;
ll n;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n;
    for(ll i = 0;i<n;++i){
        for(ll j = 0;j<n;++j){
            ll temp = abs(i - j);
            cout<<temp<<" ";
        }
        cout<<"\n";
    }

    return 0;
}