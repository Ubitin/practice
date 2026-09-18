#include<bits/stdc++.h>
using namespace std;
int main(){

    ios::sync_with_stdio(false);

    size_t n = 0;
    cin>>n;

    vector<int>v;
    v.resize(n);
    for(size_t i = 0;i<n;++i) cin>>v[i];

    sort(v.begin(),v.end());

    v.erase(unique(v.begin(),v.end()),v.end());

    size_t a = v.size();

    cout<<a<<"\n";

    for(auto iter = v.begin();iter != v.end();++iter){
        cout<<*iter<<" ";
    }

    return 0;
}