#include<bits/stdc++.h>
using namespace std;
using ll = long long;
ll n;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n;
    double ans = 0.0;
    for(ll i = 1;i<=n;++i){
        double temp;
        ll t = (i + 1)/10 + 1;
        temp = i + (i + 1) * pow(10,-t);
        ans += temp;
        cout<<temp;
        if(i < n) cout<<"+";
    }
    cout<<"="<<ans;

    return 0;
}