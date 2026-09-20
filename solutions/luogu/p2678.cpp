#include<bits/stdc++.h>
using namespace std;

long long len,n,m;
vector<long long>d;

bool p(long long x){
    long long res = 0;
    long long temp = 0;
    for(int i = 0;i<n;++i){
        if(d[i] >= temp + x){
            temp = d[i];
            res ++;
        }
    }

    long long delta = len - temp;
    
    if(delta < x && res == (n - m)) return false;

    return res >= (n - m);
}

long long find(long long l,long long r){
    long long mid = (l + r) >> 1;
    long long ans = 0;

    while(l <= r){
        if(p(mid = (l + r) >> 1)){
            ans = mid;
            l = mid + 1;
        }
        else r = mid - 1;
    }
    return ans;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>len>>n>>m; d.resize(n);
    for(int i = 0;i<n;++i) cin>>d[i];

    long long l = 0,r = len;
    long long ans = find(l,r);

    cout<<ans;

    return 0;
}