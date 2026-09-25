#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const ll maxn = 1e6;
ll n;ll a[maxn]; ll cnt = 0, sum;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n;
    for(ll i = 0;i<n;++i){
        ll x; cin>>x;
        a[i] = abs(x); sum += a[i];
        if(x <= 0) cnt ++;
    }
    
    sort(a,a + n);
    if(cnt % 2) sum -= 2 * a[0];
    
    cout<<sum;

    return 0;
}