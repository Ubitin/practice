#include<bits/stdc++.h>
using namespace std;

int val(string s,char c,size_t len){
    size_t i = 0;
    for(size_t j=0;j<len;++j){
        if(c==s[j]) ++i;
    }
    return i;
}

int f(int x){
    int b = 1;
    if(x<2) b=0;
    else if(x==2) b=1;
    else if(x>2){
        for(int j = 2;j<x;j++)
        {
            if(x%j==0) b=0;
        }
    }
    return b;
}

int main(){
    string s;
    cin>>s;
    int b;

    vector<int> v;
    v.resize(s.size());
    for(size_t i =0;i<s.size();++i) v[i] = val(s,s[i],s.size());
    sort(v.begin(),v.end());
    int x = v[v.size()-1] -v[0];
    b = f(x);
    if(b==0) cout<<"No Answer\n"<<0;
    else if(b==1) cout<<"Lucky Word\n"<<x;
    return 0;    
}