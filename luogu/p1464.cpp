#include<bits/stdc++.h>
using namespace std;
long long f[21][21][21];

long long w(long long a,long long b,long long c){

    if(a <= 0 || b <= 0 || c <= 0) return 1;

    else if(a > 20 || b > 20 || c > 20) return w(20,20,20);

    else if(f[a][b][c] != 0) return f[a][b][c];
    else if(a < b && b < c) f[a][b][c] = w(a,b,c - 1) + w(a,b - 1,c - 1) - w(a,b - 1,c - 1);
    
    else f[a][b][c] = w(a - 1,b,c) + w(a - 1,b - 1,c) + w(a - 1,b,c - 1) - w(a - 1,b - 1,c - 1);

    return f[a][b][c];
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    memset(f,21*21*21,0);//创建一个从f[1][1][1]到f[20][20][20]的数组用于储存数据，避免重复计算而超时

    long long a = 0,b = 0,c = 0;
    while(cin>>a>>b>>c){
        if(a == -1 && b == -1 && c == -1) break;

        else cout<<"w("<<a<<", "<<b<<", "<<c<<") = "<<w(a,b,c)<<"\n";
    }

    return 0;
}