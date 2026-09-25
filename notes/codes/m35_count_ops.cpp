// 量一下原码"逐个枚举"到底要跑多少次循环（不真跑，只计数，避免浪费时间）
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main() {
    ll T = 100000, n = 1000000000LL;
    printf("T=%lld 组，每组 n=%lld\n", T, n);
    printf("原码 for(i=3;i<n;++i) 的循环次数 = T * (n-3) ≈ %.3e 次\n", (double)T * (double)(n - 3));
    printf("按本机约 1e9 次简单循环/秒估算，需要约 %.1f 小时\n",
           (double)T * (double)(n - 3) / 1e9 / 3600.0);
    printf("而公式解只做 %lld 组 × 3 次四则运算 ⇒ 实测 50ms\n", T);
    return 0;
}
