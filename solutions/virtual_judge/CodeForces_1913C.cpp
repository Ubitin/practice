#include<bits/stdc++.h>
using namespace std;
using ll = long long;
 
ll m;
ll con[32];
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>m;
    
    memset(con,0,sizeof(con));
 
    while(m--){
        int t ; ll v;
        cin>>t>>v;
        bool p = true;
        
        if(t == 1) con[v] ++ ;
        
        else{
            ll has = 0;
            for(ll i = 0;i<=30;++i){
                has += con[i];
                if((v >> i) & 1){
                    if(has == 0){
                        p = false;
                        break;
                    } 
                    --has;
                }
                has >>= 1;
            }
            cout<<(p == true ? "YES\n" : "NO\n");
        }
    }
 
    return 0;
}