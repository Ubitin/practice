#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int ans = 0;
    int n;cin>>n;

    for(int i = 0;i<n;++i){
        double a; int t;
        cin>>a>>t;
        for(int j = 1;j<=t;++j){
            int b = (int)floor(a*j);
            ans ^= b;
        }
    }

    cout<<ans;
    return 0;
}