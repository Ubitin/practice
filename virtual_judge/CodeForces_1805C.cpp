#include<bits/stdc++.h>
using namespace std;

struct pao{
    long long a,b,c;
};

bool delta(long long a,long long b,long long c,long long k){
    if(((b - k) * (b - k) - 4 * a * c ) < 0) return true;

    return false;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 0; cin>>t;

    while(t--){
        int n,m; cin>>n>>m;
        vector<long long>k(n); vector<pao>p(m);

        for(int i = 0;i<n;++i) cin>>k[i];
        for(int i = 0;i<m;++i) cin>>p[i].a>>p[i].b>>p[i].c;

        sort(k.begin(),k.end());

        for(int i = 0;i<m;++i){
            long long ans = 0; int cnt = 0;

            auto iter = lower_bound(k.begin(),k.end(),p[i].b);

            for(auto temp : {iter, iter == k.begin() ? k.end() : prev(iter)}){
                if(temp == k.end()) continue;

                if(delta(p[i].a,p[i].b,p[i].c,*temp)) {
                    ans = *temp; cnt ++;
                }
            }

            if(cnt) cout<<"YES\n"<<ans<<"\n";
            else cout<<"NO\n";
        }

        cout<<"\n";
    }

    return 0;
}