#include<bits/stdc++.h>
using namespace std;
using ll = long long;

char trans(char m){
    if(m == ')') return '(';
    else if(m == ']') return '[';
    else if(m == '}') return '{';

    return '\0';
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll t ; cin>>t;
    while(t--){
        stack<char> s;
        string num; cin>>num;
        ll lenth = num.size();
        for(ll i = 0;i<lenth;++i){
            char c; c = num[i];
            if(s.empty()) s.push(c);
            else{
                if(trans(c) == s.top()) s.pop();
                else s.push(c);
            }
        }
        if(s.empty()) cout<<"Yes\n";
        else cout<<"No\n";
    }

    return 0;
}