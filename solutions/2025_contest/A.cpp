#include<bits/stdc++.h>
using namespace std;
using ll = long long;

ll a[3];ll d1,d2;ll ans = 0;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    for(int i = 0;i<3;++i) cin>>a[i];

    d1 = a[1] - a[0]; d2 = a[2] - a[1];

    if(d1 == d2) ans = 0;

    else if(d1 > d2){
        ans += d1 - d2;
    }

    else if(d1 < d2){
        while(d1 < d2){
            ans ++;
            d1 ++; d2 --;
        }
        if(d1 > d2) ans ++;
    }

    cout<<ans;

    return 0;
}