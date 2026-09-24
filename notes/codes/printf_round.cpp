// 再补一格：printf("%.0f") 的 .5 到底怎么走？（C 标准：按当前舍入模式，默认"就近取偶"）
#include <bits/stdc++.h>
using namespace std;
int main() {
    double xs[] = {0.5, 1.5, 2.5, 3.5, 12.5, 13.5, -0.5, -1.5, -2.5, -12.5, -12.7, 12.7};
    printf("%-8s %-14s %-12s %-14s\n", "x", "printf(\"%.0f\")", "llround", "(ll)(x+0.5)");
    for (double x : xs) {
        char buf[64];
        snprintf(buf, sizeof buf, "%.0f", x);
        printf("%-8.4g %-14s %-12lld %-14lld\n", x, buf, llround(x), (long long)(x + 0.5));
    }
    printf("\n读法：printf 用\"就近取偶\"（12.5→12、13.5→14），llround 用\"远离 0\"（12.5→13）。\n");
    return 0;
}
