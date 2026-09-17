#include<bits/stdc++.h>
using namespace std;

bool prime(int x){
    if(x < 2) return false;                       // 0、1 不是素数
    if(x % 2 == 0) return x == 2;                 // 偶数里只有 2 是素数
    for(int i = 3;(long long)i*i <= x;i += 2)     // 只试奇数；用 <=（否则平方数漏判）
        if(x % i == 0) return false;              // ⚠️ 括号！x % i == 0 而不是 !x%i
    return true;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n,k; cin>>n>>k;
    vector<int>a(n);
    for(int i = 0;i < n;++i) cin>>a[i];

    int cnt = 0;
    for(int mask = 0;mask < (1<<n);++mask){       // 直接枚举掩码，不必再存 vector
        if(__builtin_popcount(mask) != k) continue;   // 恰好选 k 个
        int sum = 0;                              // 最大 20×5e6 = 1e8，int 够
        for(int j = 0;j < n;++j)
            if((mask >> j) & 1) sum += a[j];
        if(prime(sum)) ++cnt;
    }

    cout<<cnt<<"\n";
    return 0;
}