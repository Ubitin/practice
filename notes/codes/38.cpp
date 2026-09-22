// (1 << 29) 到底超没超 long long？以及真正的坑在哪里 —— 全部实测
#include <bits/stdc++.h>
using namespace std;

int main() {
    printf("sizeof(int)=%zu  sizeof(long long)=%zu\n", sizeof(int), sizeof(long long));
    printf("INT_MAX    = %d\n", INT_MAX);
    printf("LLONG_MAX  = %lld\n", LLONG_MAX);
    printf("\n");

    // ---------- ① 常量形式：编译器能算出结果，会直接给警告 ----------
    printf("===== ① 常量写法（编译期就能算出）=====\n");
    printf("1 << 10 = %d\n", 1 << 10);
    printf("1 << 20 = %d\n", 1 << 20);
    printf("1 << 29 = %d          <- 你问的这个\n", 1 << 29);
    printf("1 << 30 = %d\n", 1 << 30);
    printf("\n");

    // ---------- ② 运行时形式：用 volatile 挡住编译期常量折叠 ----------
    volatile int n29 = 29, n30 = 30, n31 = 31, n32 = 32, n40 = 40, n62 = 62, n63 = 63, n64 = 64;
    printf("===== ② 运行时写法（volatile 挡住常量折叠，看真实行为）=====\n");
    printf("1 << 29 = %d\n", 1 << n29);
    printf("1 << 30 = %d\n", 1 << n30);
    printf("1 << 31 = %d   <- int 装不下了（有符号左移溢出）\n", 1 << n31);
    printf("1 << 32 = %d   <- 移位位数 >= 类型宽度\n", 1 << n32);
    printf("1 << 40 = %d\n", 1 << n40);
    printf("1LL << 29 = %lld\n", 1LL << n29);
    printf("1LL << 40 = %lld\n", 1LL << n40);
    printf("1LL << 62 = %lld\n", 1LL << n62);
    printf("1LL << 63 = %lld   <- long long 也装不下了\n", 1LL << n63);
    printf("1LL << 64 = %lld   <- 移位位数 >= 64\n", 1LL << n64);
    printf("\n");

    // ---------- ③ 最经典的坑：算完存进 long long ----------
    printf("===== ③ 存进 long long 也没用：先算的那一步已经在 int 里溢出了 =====\n");
    long long a = 1 << n31;              // 先算 int 的 1<<31，再转成 long long
    long long b = 1LL << n31;            // 一开始就是 long long
    printf("long long a = (1 << 31)   -> %lld   （负的！）\n", a);
    printf("long long b = (1LL << 31) -> %lld\n", b);
    printf("long long c = (1 << 29)   -> %lld   （你问的这个，没问题）\n", (long long)(1 << n29));
    printf("\n");

    // ---------- ④ 无符号 / long long 的对照 ----------
    printf("===== ④ 换成 unsigned / long long 就没事 =====\n");
    printf("1u  << 31 = %u\n", 1u << n31);
    printf("1LL << 31 = %lld\n", 1LL << n31);
    printf("1LL << 32 = %lld\n", 1LL << n32);
    printf("\n");

    // ---------- ⑤ 与"枚举子集"直接相关：1<<n 的可用边界 ----------
    printf("===== ⑤ 枚举子集时 1<<n 的边界（运行时 n）=====\n");
    volatile int kk;
    for (int k = 28; k <= 33; ++k) {
        kk = k;
        printf("  n=%2d : 1<<n = %-12d   1LL<<n = %-20lld\n", k, 1 << kk, 1LL << kk);
    }
    return 0;
}
