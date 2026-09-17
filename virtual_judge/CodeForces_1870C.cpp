#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int T = 0; cin>>T;
    while(T--){
        int n = 0,k = 0; cin>>n>>k;
        vector<int>L(k+2,n+1); vector<int>R(k+2,0); vector<int>has(k+2,0);
 
        for(int i = 0;i<n;++i){
            int a = 0; cin>>a;
            
            L[a] = min(L[a],i);
            R[a] = max(R[a],i);
            has[a] = 1;
        }
 
        for(int j = k - 1;j>=1;--j){
            L[j] = min(L[j],L[j+1]);
            R[j] = max(R[j],R[j+1]);
        }
 
        for(int i = 1;i<=k;++i){
            if(!has[i]) cout<<0<<" ";
            else cout<<2*(R[i] - L[i] + 1)<<" ";
        }
 
        cout<<"\n";
    }
    return 0;
}