#include<bits/stdc++.h>
using namespace std;

long long m,n;
vector<long long>a;

long long find(long long b){
    auto iter = lower_bound(a.begin(),a.end(),b);
    long long res = 1e9;
    for(auto x : {iter,iter == a.begin() ? a.end() : prev(iter)}){
        if(x == a.end()) continue;
        long long temp = abs(*x - b);
        res = min(res,temp);
    }

    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>m>>n; a.resize(m);
    for(int i = 0;i<m;++i) cin>>a[i];
    sort(a.begin(),a.end());

    vector<long long>b(n);
    long long ans = 0;
    for(int i = 0;i<n;++i){
        cin>>b[i];
        ans += find(b[i]);
    }

    cout<<ans<<"\n";

    return 0;
}