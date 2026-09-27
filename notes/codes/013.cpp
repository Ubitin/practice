#include<bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll a , b , p;
    cin>>a>>b>>p;
    ll temp = pow(a,b);
    ll ans = temp % p;

    cout<<ans;

    return 0;
}