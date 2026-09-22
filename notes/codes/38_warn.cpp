// 常量写法：看编译器给不给警告（这是发现这类 bug 的第一道防线）
#include <bits/stdc++.h>
using namespace std;
long long g1 = 1 << 31;      // 有符号左移溢出
long long g2 = 1 << 32;      // 移位位数 ≥ 类型宽度
long long g3 = 1LL << 63;    // long long 溢出
long long g4 = 1LL << 64;    // 移位位数 ≥ 64
long long g5 = 1 << 29;      // 正常的
long long g6 = 1LL << 62;    // long long 里最大的 2 的幂
int main() { printf("%lld %lld %lld %lld %lld %lld\n", g1, g2, g3, g4, g5, g6); return 0; }
