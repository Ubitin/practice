#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main(){
    ll n; if(!(cin>>n)) return 0;
    vector<ll> a(n+1);
    for(ll i=1;i<=n;++i) cin>>a[i];
    ll q; cin>>q;
    vector<ll> typ(q), p(q,0), x(q,0);
    for(ll i=0;i<q;++i){ cin>>typ[i]; if(typ[i]==1){ cin>>p[i]>>x[i]; } else { cin>>x[i]; } }
    cerr << "n=" << n << " q=" << q << "\n";
    for(ll i=0;i<q;++i) cerr << "  op" << i+1 << ": type=" << typ[i] << " p=" << p[i] << " x=" << x[i] << "\n";
    for(ll i=1;i<=n;++i) cerr << "  a[" << i << "]=" << a[i] << "\n";

    vector<ll> sub(n+1,0); vector<char> seen(n+1,0); ll mx=0;
    for(ll i=q-1;i>=0;--i){
        if(typ[i]==2){ mx=max(mx,x[i]); cerr<<"  rev op"<<i+1<<" type2 x="<<x[i]<<" -> mx="<<mx<<"\n"; }
        else { ll pp=p[i]; if(!seen[pp]){ seen[pp]=1; sub[pp]=mx; a[pp]=x[i];
               cerr<<"  rev op"<<i+1<<" type1 p="<<pp<<" x="<<x[i]<<" -> sub="<<sub[pp]<<" a="<<a[pp]<<"\n"; } }
    }
    for(ll i=1;i<=n;++i) cerr<<"  final i="<<i<<" a="<<a[i]<<" sub="<<sub[i]<<" ans="<<max(a[i],sub[i])<<"\n";
    for(ll i=1;i<=n;++i) cout<<max(a[i],sub[i])<<" \n"[i==n];
    return 0;
}
