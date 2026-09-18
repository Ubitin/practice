#include<bits/stdc++.h>
using namespace std;

struct n{
    string name;
    int val;
};

int sum(string a){
    int res = 0;
    
    for(size_t i = 0;i<a.size();++i) res += a[i];

    return res;
}

int main(){

    int t = 0;
    cin>>t;

    vector<struct n>a;
    a.resize(t);

    for(int i = 0;i<t;++i){
        cin>>a[i].name;
        a[i].val = sum(a[i].name);
    }

    for(int i = 0;i<t;++i){
        switch(a[i].val % 3){
            case 0:
            cout<<"Case "<<i + 1<<":"<<" Yes"<<"\n";
            break;
            default:
            cout<<"Case "<<i + 1<<":"<<" No"<<"\n";
            break;
        }
    }

    return 0;
}