#include<bits/stdc++.h>
using namespace std;

int num(int n){
    int res = 1;
    int t = floor(n/2);

    if(n == 1) return 1;

    for(int i = 1;i<=t;++i){
        res += num(i);
    }

    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n = 0; cin>>n;

    cout<<num(n);

    return 0;
}