#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n = 0; cin>>n;
    vector<int>a;
    a.resize(n);
    for(int i = 0;i<n;++i) cin>>a[i];

    sort(a.begin(),a.end());

    int cnt = 0;

    vector<int>sum;
    sum.resize(n*(n-1));

    int id = 0;
    for(int i = 0;i<n;++i){
        for(int j = i+1;j<n;++j){
            sum[id] = a[i] + a[j];
            id++;
        }
    }
    sort(sum.begin(),sum.end());

    sum.erase(unique(sum.begin(),sum.end()),sum.end());

    for(int i = 0;i<n;++i){
        for(size_t j = 0;j<sum.size();++j){
            if(a[i] == sum[j]) cnt++;
        }
    }
    
    cout<<cnt;
    return 0;
}