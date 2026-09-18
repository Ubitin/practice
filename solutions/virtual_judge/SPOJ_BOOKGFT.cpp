#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);

    int t = 0;
    cin>>t;

    vector<pair<int,int>>p;
    p.resize(t);

    for(int i = 0;i<t;i++) cin>>p[i].first>>p[i].second;

    for(int i = 0;i<t;++i){
        if((p[i].second%p[i].first) == 0) cout<<"YES"<<"\n";
        else cout<<"NO"<<"\n";
    }
    
    return 0;
}