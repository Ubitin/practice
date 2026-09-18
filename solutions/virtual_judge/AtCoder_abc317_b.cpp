#include<bits/stdc++.h>
using namespace std;


int main(){

    int n;
    cin>>n;
    
    vector<int>v;
    v.resize(n);
    for(int i = 0;i<n;++i){
        cin>>v[i];
    }

    sort(v.begin(),v.end());

    for(int i = 0;i<n - 1;++i){
        if(fabs(v[i] - v[i + 1]) != 1){
            cout<<v[i] + 1;
        }
    }

    return 0;
}