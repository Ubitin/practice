#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n; cin>>n;

    vector<long long>a(n,0);
    long long ans = 0;
    for(long long i = 0;i<n;++i) cin>>a[i];
    for(long long i = 1;i<n;++i){
        if(a[i] > a[i - 1]){
            ans += (a[i] - a[i - 1]);
        }
    }
    cout<<ans + a[0];

    return 0;
}