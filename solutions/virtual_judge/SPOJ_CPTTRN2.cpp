#include<bits/stdc++.h>
using namespace std;

struct a{
    int c,l;
};

int main(){

    ios::sync_with_stdio(false);
    int t = 0;
    cin>>t;

    vector<struct a>v;
    v.resize(t);
    for(int i = 0;i<t;++i) cin>>v[i].c>>v[i].l;

    for(int i = 0;i<t;++i){
        for(int j = 0;j<v[i].c;++j){
            for(int k = 0;k<v[i].l;++k){
                if(j == 0||j == (v[i].c - 1)||k == 0||k == v[i].l - 1) cout<<"*";
                else cout<<".";
            }
            cout<<"\n";
        }
        cout<<"\n";
    }

    return 0;
}