#include<bits/stdc++.h>
using namespace std;
int main(){
    int x;
    int apple[10];
    for(int i = 0; i < 10; ++i) cin >> apple[i];
    cin >> x;
    int val = 0;                       // ← 关键修复：初始化！
    for(int i = 0; i < 10; ++i)
        if(apple[i] <= x + 30) val++;  // 逻辑简化：直接用 if 计数
    cout << val;
    return 0;
}
