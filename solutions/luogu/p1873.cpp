#include<bits/stdc++.h>
using namespace std;

long long n,h;
vector<long long>v;

bool p(long long x){
    long long sum = 0;
    
    for(int i = 0;i<n;++i){
        if(v[i] > x) sum += (v[i] - x);
    }

    return sum >= h;
}

long long find(long long l ,long long r){
    long long mid = (l + r) >> 1;

    long long ans = 0;
    while(l <= r){
        if(p(mid = (l + r) >> 1)){
            ans = mid;
            l = mid + 1;
        }
        else r = mid - 1;
    }

    return ans ;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n>>h;
    v.resize(n);
    for(int i = 0;i<n;++i) cin>>v[i];

    long long l = 0, r = 2e9;
    long long ans = find(l,r);

    cout<<ans;

    return 0;
}