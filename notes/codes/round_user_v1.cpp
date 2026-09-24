#include<bits/stdc++.h>
using namespace std;

long long g(double x){//实现四舍五入
    long long u = x;
    long long temp = x * 10;
    temp %= 10;
    if(temp < 5) return u;
    else if(temp >= 5) return u + 1;
}

long long f(long long t,long long v){//翻译条件
    double w = 13.12 + 0.6215 * t - 11.37 * pow(v,0.16) + 0.3665 * t * pow(v , 0.16);
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