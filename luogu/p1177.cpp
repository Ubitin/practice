#include<bits/stdc++.h>
using namespace std;
int main(){
    int n;
    cin>>n;
    vector<int>v(n);
    for(int i = 0;i<n;++i) {cin>>v[i];}
    sort(v.begin(),v.end());
    for(int j = 0;j<n;++j){cout<<v[j]<<" ";}
    return 0;
}