#include<bits/stdc++.h>
using namespace std;
using ll = long long;
string s1,s2;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>s1>>s2;
    ll ans = 0;
    for(auto iter = s1.begin();iter != s1.end();++iter) {
        auto pos = s2.find(*iter);
        if(next(iter) == s1.end()) continue;
        if(*(next(iter)) == s2[pos - 1]) ans++;
    }

    cout<<(1 << ans);

    return 0;
}