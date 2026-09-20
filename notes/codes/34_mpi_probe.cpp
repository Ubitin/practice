// test_/34_mpi_probe.cpp —— 探测 M_PI 在不同 -std 下的可用性
// 配套讲义：讲义/反三角函数详解-asin-acos-atan-atan2.md
// 本文件为新增探针程序，不改动任何既有文件。
#include <bits/stdc++.h>   // OJ 常用万能头；此处用于验证"换成万能头会不会改变结论"
#include <cstdio>
int main() {
#ifdef M_PI
    printf("M_PI 已定义      = %.15f\n", M_PI);
#else
    printf("M_PI 未定义      <-- 编译期就找不到这个宏\n");
#endif
#ifdef _USE_MATH_DEFINES
    printf("_USE_MATH_DEFINES 已定义\n");
#else
    printf("_USE_MATH_DEFINES 未定义\n");
#endif
    printf("acos(-1.0) 替代  = %.15f\n", acos(-1.0));
    return 0;
}
