#include<bits/stdc++.h>
using namespace std;
using ll = long long;
queue<ll>q;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n,m; cin>>n>>m; 
    for(ll i = 1;i<= n;++i) q.push(i);

    while(n--){
        for(ll i = 0;i<m;++i){
            if(i != m - 1){
                ll temp = q.front(); q.pop();
                q.push(temp);
            }
            else {
                cout<<q.front()<<" "; q.pop();
            }
        }
    }

    return 0;
}