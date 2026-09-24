// 四种"四舍五入"写法的对照：谁对负数也对？谁对 .5 怎么处理？
#include <bits/stdc++.h>
using namespace std;

long long g_user(double x) {              // 你写在 01.cpp 里的
    long long u = x;
    long long temp = x * 10;
    temp %= 10;
    if (temp < 5) return u;
    else if (temp >= 5) return u + 1;
}
long long g_round(double x) { return llround(x); }              // 标准库：.5 远离 0
long long g_plus(double x) { return (long long)(x + 0.5); }     // 常见土办法：.5 向上
long long g_floor(double x) { return (long long)floor(x + 0.5); } // 仅对非负数正确

int main() {
    printf("%-10s %-12s %-12s %-12s %-12s %s\n", "x", "你的g", "llround", "(ll)(x+.5)", "floor(x+.5)", "谁和谁不一样");
    double xs[] = {12.7, 12.5, 12.4, -12.4, -12.5, -12.7, 0.5, -0.5, -0.6, -37.77, -31.535, 0.0};
    for (double x : xs) {
        long long a = g_user(x), b = g_round(x), c = g_plus(x), d = g_floor(x);
        printf("%-10.4g %-12lld %-12lld %-12lld %-12lld", x, a, b, c, d);
        if (a != b) printf("  你的 != llround");
        if (b != c) printf("  llround != (ll)(x+.5)");
        printf("\n");
    }

    printf("\n=== 负数网格 k+d/10（k=-30..-1, d=0..9）：与 llround 比对 ===\n");
    int wUser = 0, wPlus = 0, wFloor = 0, tot = 0;
    for (int k = -30; k <= -1; ++k)
        for (int d = 0; d <= 9; ++d) {
            double x = k + d / 10.0;
            ++tot;
            if (g_user(x) != g_round(x)) ++wUser;
            if (g_plus(x) != g_round(x)) ++wPlus;
            if (g_floor(x) != g_round(x)) ++wFloor;
        }
    printf("  共 %d 个负数：你的g 错 %d 个， (ll)(x+.5) 错 %d 个， floor(x+.5) 错 %d 个\n",
           tot, wUser, wPlus, wFloor);

    printf("\n=== 非负网格 k+d/10（k=0..40, d=0..9）：与 llround 比对 ===\n");
    wUser = wPlus = wFloor = tot = 0;
    for (int k = 0; k <= 40; ++k)
        for (int d = 0; d <= 9; ++d) {
            double x = k + d / 10.0;
            ++tot;
            if (g_user(x) != g_round(x)) ++wUser;
            if (g_plus(x) != g_round(x)) ++wPlus;
            if (g_floor(x) != g_round(x)) ++wFloor;
        }
    printf("  共 %d 个非负数：你的g 错 %d 个， (ll)(x+.5) 错 %d 个， floor(x+.5) 错 %d 个\n",
           tot, wUser, wPlus, wFloor);
    return 0;
}
