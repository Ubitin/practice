#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 2147483647;
const ll maxn = 1e4;
bool b[maxn];

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n; cin>>n;
    vector<ll>v;
    b[0] = 1;
    for(ll i = 1;i<=n;++i) {
        ll opt , x; cin>>opt>>x;
        if(opt == 5) b[i] = 1;
        else b[i] = 0;
        
        if(b[i - 1] == 1 && b[i] == 0) sort(v.begin(),v.end());
        
        if(opt == 5) {
            v.push_back(x);
        }
        

        else if(opt == 1) {
            auto iter = lower_bound(v.begin(),v.end(),x);
            if(iter == v.begin()) cout<<1<<"\n";
            else {
                iter--;
                cout<<iter - v.begin() + 2<<"\n";
            }
        }
        else if(opt == 2) {
            cout<<v[x - 1]<<"\n";
        }
        else if(opt == 3) {
            auto iter = lower_bound(v.begin(),v.end(),x);
            if(iter == v.begin()) cout<<-INF<<"\n";
            else if(iter == v.end()) {
                cout<<v.back()<<"\n";
            }
            else {
                iter--;
                cout<<*iter<<"\n";
            }
        }
        else if(opt == 4) {
            auto iter = upper_bound(v.begin(),v.end(),x);
            if(iter == v.end()) cout<<INF<<"\n";
            else {
                cout<<*iter<<"\n";
            }
        }
    }

    return 0;
}