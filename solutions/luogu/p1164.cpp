#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n,m; cin>>n>>m;

    vector<int>dp(m + 1,0);

    dp[0] = 1;
    for(int i = 0;i<n;++i){
        int x = 0; cin>>x;

        for(int j = m;j>=x;--j){
            dp[j] += dp[j - x];
        }
    }

    cout<<dp[m];

    return 0;

}