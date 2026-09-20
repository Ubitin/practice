#include<bits/stdc++.h>
using namespace std;
int t[10];

void f1(long long x){                 // 前置条件：x 必须是三位数
    t[x%10] = 1;                      // 个位
    t[x/10%10] = 1;                   // 十位
    t[x/100] = 1;                     // 百位
}

bool f2(long long x,long long y,long long z){
    memset(t,0,sizeof(t));
    f1(x),f1(y),f1(z);
    for(int i = 1;i <= 9;++i)
        if(!t[i]) return false;       // 去掉原来那句永远执行不到的 break
    return true;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a,b,c,cnt = 0;
    cin >> a >> b >> c;

    for(long long x = 123;x <= 987;++x){
        if(x*b%a || x*c%a) continue;                  // 除不尽就跳过
        long long y = x*b/a, z = x*c/a;               // 先乘后除
        if(y < 100 || y > 999 || z < 100 || z > 999) continue;   // 必须都是三位数
        if(f2(x,y,z)){
            cout << x << " " << y << " " << z << "\n";
            ++cnt;                                    // ← 只有真的输出了解才计数
        }
    }
    if(!cnt) cout << "No!!!" << "\n";
    return 0;
}
