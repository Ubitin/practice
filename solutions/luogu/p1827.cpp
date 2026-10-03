#include<bits/stdc++.h>
using namespace std;
using ll = long long;

ll a1[27];
ll a2[27];
ll trans(char c) {
    return c - 'A';
}

char retrans(ll x) {
    return 'A' + x;
}

void build(ll l1,ll r1,ll l2,ll r2) {
    for(ll i = l1; i<=r1;++i) {
        if(a1[i] == a2[l2]) {
            build(l1,i - 1,l2 + 1,l2 + i - l1);
            build(i + 1,r1,l2 + i - l1 + 1,r2);
            cout<<retrans(a2[l2]);
        }
    }   
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s1,s2; cin>>s1>>s2; ll lenth = s1.size();
    for(ll i = 0;i<lenth;++i) {
        a1[i] = trans(s1[i]); a2[i] = trans(s2[i]);
    }

    build(0,lenth - 1,0,lenth - 1);

    return 0;
}