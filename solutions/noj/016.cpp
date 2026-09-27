#include<bits/stdc++.h>
using namespace std;
using ull = unsigned long long;
using ulli128 = __int128;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ull a,b,c;
    cin>>a>>b>>c;
    ulli128 temp = (ulli128)a * b;
    ull ans = temp % c;
    cout<<ans;

    return 0;
}