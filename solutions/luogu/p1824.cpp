#include<bits/stdc++.h>
using namespace std;

vector<long long>a;
long long n,m;

bool p(long long x){
    long long k = 1;
    long long last = a[0];
    for(long long i = 1;i<n;++i){
        if((a[i] - last) >= x){
            ++k; last = a[i];
        }
    }

    return k >= m;
}

long long find(long long l,long long r){
    long long ans = 0;
    long long mid = (l + r) >> 1;
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

    cin>>n>>m;
    a.resize(n);
    for(int i = 0;i<n;++i) cin>>a[i];
    sort(a.begin(),a.end());    

    long long l = 0,r = 2e9;

    long long ans = find(l,r);

    cout<<ans;
    return 0;
}