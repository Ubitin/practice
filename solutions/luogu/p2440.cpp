#include<bits/stdc++.h>
using namespace std;
long long n,k;
vector<long long>len;

bool p(long long x){
    long long res = 0;
    if(x <= 0) return false;//保证不会有整数除以0的错误出现
    for(long long i = 0;i<n;++i){
        res += (len[i] / x);
    }
    return res >= k;
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

    return ans ;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n>>k; len.resize(n);
    for(long long i = 0;i<n;++i) cin>>len[i];

    long long l = 1,r = 1e9;//l不能取0,因为0是不合法的，会出现整数除以0的情况
    
    long long sum = 0;
    for(long long i = 0;i<n;++i) sum += len[i];
    if(sum < k) cout<<0;

    else if(sum == k) cout<<1;

    else{
        long long ans = find(l,r);

        cout<<ans;
    }

    return 0;
}