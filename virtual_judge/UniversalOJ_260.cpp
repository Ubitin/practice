#include<bits/stdc++.h>
using namespace std;

struct toy{
    int head; string job;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n = 0,m = 0; cin>>n>>m;

    vector<toy>t; t.resize(n);

    for(int i = 0;i<n;++i) cin>>t[i].head>>t[i].job;

    int x = 0;
    for(int i = 0;i<m;++i){
        int a = 0,s = 0; cin>>a>>s;
        int map = t[x].head ^ a;
        if(map) x = (x + s)%n;
        else x = (x + n - s)%n;
    }

    cout<<t[x].job;

    return 0;
}