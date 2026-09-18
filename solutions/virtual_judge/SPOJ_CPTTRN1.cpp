#include<bits/stdc++.h>
using namespace std;

struct b{
    int l , c;
};

int main(){

    ios::sync_with_stdio(false);

    int t = 0;
    cin>>t;
    vector<struct b>v;
    v.resize(t);

    for(int i = 0;i<t;++i) cin>>v[i].c>>v[i].l;


    for(int i = 0;i<t;++i){
        for(int j = 0;j<v[i].c;++j){
            for(int k = 0;k<v[i].l;++k){
                if(j%2 == 0){
                    if(k%2 == 0) cout<<"*";
                    else cout<<".";
                }
                else{
                    if(k%2 == 1) cout<<"*";
                    else cout<<".";
                }
            }
            cout<<"\n";
        }
        cout<<"\n";
    }

    return 0;
}