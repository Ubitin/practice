#include<bits/stdc++.h>
using namespace std;

int main(){

    ios::sync_with_stdio(false);
    int n = 0,k = 0;
    cin>>n>>k;

    vector<int>v;
    v.resize(n);
    for(int i = 0;i<n;++i) cin>>v[i];

    int num = 0;

    for(int i = 0;i<n;++i){
        if(v[i]%k == 0) num++;
    }

    cout<<num;

    return 0;
}