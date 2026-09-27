#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const ll maxn = 1e7;
ll n,q,opt,i,j,k;
vector<vector<ll>>locker;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n>>q;
    locker.resize(n + 1);
    while(q--){
        cin>>opt;
        if(opt == 1){
            cin>>i>>j>>k;
            if(locker[i].size() < j) locker[i].resize(j + 1);
            locker[i][j] = k;
        }
        else if(opt == 2){
            cin>>i>>j;
            cout<<locker[i][j]<<"\n";
        }
    }

    return 0;
}