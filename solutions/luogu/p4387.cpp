#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const ll maxn = 1e5 + 5;
ll q,n;
ll pu[maxn],po[maxn];

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>q;
    while(q--) {
        cin>>n;
        
        for(ll i = 0;i<n;++i) cin>>pu[i];
        for(ll i = 0;i<n;++i) cin>>po[i];
        po[n] = -1;

        ll j = 0; stack<ll>s;
        
        for(ll i = 0;i<n;++i){
            s.push(pu[i]);
            while(!s.empty() && s.top() == po[j]) {
                s.pop(); j++;
            }
        }

        cout<<(j == n ? "Yes\n" : "No\n");
    }
    return 0;
}