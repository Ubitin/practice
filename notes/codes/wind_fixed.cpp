#include<bits/stdc++.h>
using namespace std;

long long g(double x){//瀹炵幇鍥涜垗浜斿叆
    return llround(x);
}

long long f(long long t,long long v){//缈昏瘧鏉′欢
    double w = 13.12 + 0.6215 * t - 11.37 * pow(v,0.16) + 0.3965 * t * pow(v , 0.16);
    long long temp = g(w);
    return temp;
}


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long t,v; cin>>v>>t;

    long long ans = f(t,v);
    cout<<ans;

    return 0;
}
