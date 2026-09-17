#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long ans = 0;
    long long n,x; cin>>n>>x;

    vector<long long>v(n);
    for(long long i = 0;i<n;++i) cin>>v[i];

    sort(v.begin(),v.end());

    for(long long i = 0;i<n;++i){
        long long t = v[i] + x;
        ans += upper_bound(v.begin(),v.end(),t) - lower_bound(v.begin(),v.end(),t);
    }

    cout<<ans;

    return 0;
}