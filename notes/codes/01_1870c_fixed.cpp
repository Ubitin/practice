#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; cin>>T;
    while(T--){
        int n,k; cin>>n>>k;
        vector<int> L(k+2, n+1), R(k+2, 0), has(k+2, 0);
        for(int i = 1;i <= n;++i){
            int v; cin>>v;
            has[v] = 1;                       // 颜色 v 出现过
            L[v] = min(L[v], i);
            R[v] = max(R[v], i);
        }
        // 后缀合并：L[x]/R[x] 变成"所有值 ≥ x 的位置"的最左/最右
        for(int x = k-1;x >= 1;--x){
            L[x] = min(L[x], L[x+1]);
            R[x] = max(R[x], R[x+1]);
        }
        for(int x = 1;x <= k;++x){
            if(!has[x]) cout<<0<<' ';         // 颜色 x 没出现 → 0
            else cout<<2*(R[x]-L[x]+1)<<' ';
        }
        cout<<"\n";
    }
    return 0;
}
