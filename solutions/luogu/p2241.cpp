#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long m,n; cin>>m>>n;

    long long squar = 0,t = 0;

    for(long long x = 0;x<=m;++x){
        for(long long y = 0;y<=n;++y){
            squar += min(x,y);
            t += (x*y - min(x,y));
        }
    }

    cout<<squar<<" "<<t<<"\n";

    return 0;
}