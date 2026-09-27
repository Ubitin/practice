#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main(){
    ll n; if(!(cin>>n)) return 0;
    vector<ll> a(n+1), t(n+1,0);
    for(ll i=1;i<=n;++i) cin>>a[i];
    ll q; cin>>q;
    vector<ll> mon(q+2,0);
    for(ll i=1;i<=q;++i){ ll k; cin>>k;
        if(k==1){ ll p,x; cin>>p>>x; a[p]=x; t[p]=i; }
        else { ll x; cin>>x; mon[i]=x; } }
    vector<ll> suf(q+2,0);
    for(ll i=q;i>=1;--i) suf[i]=max(suf[i+1],mon[i]);

    fprintf(stderr,"n=%lld q=%lld\n",n,q);
    fprintf(stderr,"a[]  ="); for(ll i=1;i<=n;++i) fprintf(stderr," %lld",a[i]); fprintf(stderr,"\n");
    fprintf(stderr,"t[]  ="); for(ll i=1;i<=n;++i) fprintf(stderr," %lld",t[i]); fprintf(stderr,"\n");
    fprintf(stderr,"mon[]="); for(ll i=1;i<=q;++i) fprintf(stderr," %lld",mon[i]); fprintf(stderr,"\n");
    fprintf(stderr,"suf[]="); for(ll i=1;i<=q;++i) fprintf(stderr," %lld",suf[i]); fprintf(stderr,"\n");
    fprintf(stderr,"ans  =");
    for(ll i=1;i<=n;++i) fprintf(stderr," %lld",max(a[i],suf[t[i]]));
    fprintf(stderr,"\n");
    for(ll i=1;i<=n;++i) cout<<max(a[i],suf[t[i]])<<" \n"[i==n];
    return 0;
}
