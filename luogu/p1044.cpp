#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n = 0; cin>>n;

    int sol[n + 1] = {1,1};

    for(int i = 2;i<=n;++i){
        for(int j = 0;j<i;++j){
            sol[i] += sol[j] * sol[i - j - 1];
        }
    }
    cout<<sol[n];

    return 0;
}