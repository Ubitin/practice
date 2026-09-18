#include<bits/stdc++.h>
using namespace std;
int su(vector<int>v,int i,int m){
    int res = 0;
    
    for(int k = 0;k<m;++k){
        res+=v[i]; i++;
    }
    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n,m; cin>>n>>m;

    vector<int>v;
    v.resize(n);

    for(int i = 0;i<n;++i) cin>>v[i];

    vector<int>sum;
    sum.resize(n-m+1);

    for(int i = 0;i<n-m+1;++i) sum[i] = su(v,i,m);

    sort(sum.begin(),sum.end());
    cout<<sum[0];

    return 0;
}