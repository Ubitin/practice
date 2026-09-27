#include<bits/stdc++.h>
using namespace std;
using ll = long long;

long long sum(long long n,long long k){
    long long ans = 0;
    long long t = (n - 1) / k;
    ans = k * t + t * (t - 1)  * k / 2 ;
    return ans;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long T; cin>>T;
    while(T --){
        long long n; cin>>n;
        long long s1 = sum(n,3);
        long long s2 = sum(n,5);
        long long s3 = sum(n,15);
        cout<<s1 + s2 - s3<<"\n";
    }

    return 0;
}