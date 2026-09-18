#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n,x; cin>>n>>x;
    vector<int>c(n,0);
    for(long long i = 0;i<n;++i) cin>>c[i];

    long long ans = 0;

    for(long long i = 1;i<n;++i){
        if(c[i - 1] + c[i] > x){
            long long delta = c[i - 1] + c[i] - x;
            ans += delta;
            if(c[i] >= delta) c[i] -= delta;
            else{
                c[i - 1] -= (delta - c[i]);
                c[i] = 0;
            }
        }
    }

    cout<<ans;

    return 0;
}