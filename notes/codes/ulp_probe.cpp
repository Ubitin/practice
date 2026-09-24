// 验证"卡死"的机理：看 del=1e-11 与该量级下 1 个 ULP 的关系
#include <bits/stdc++.h>
using namespace std;
int main() {
    double del = 1e-11;
    double xs[] = {22500, 31684, 32761, 32768, 33124, 34225, 40000, 90000};
    printf("del = 1e-11 = %.6e\n\n", del);
    printf("%10s %14s %14s %14s %s\n", "x", "1个ULP", "del/ULP", "x-del 实际减掉", "能否收敛");
    for (double x : xs) {
        double ulp = nextafter(x, 1e300) - x;
        double y = x - del;                 // 循环里真正的写法
        printf("%10.0f %14.4e %14.2f %14.4e   %s\n",
               x, ulp, del / ulp, x - y,
               (x - y) >= del ? "可以" : "✘ r-del 被舍入成 r-1ULP");
    }
    printf("\n读法：循环条件 while (l <= r - del) 只有在 r-del 真的比 r 小 del 时才可能退出。\n");
    printf("      当 del/ULP < 2 时，x-del 实际只减掉 1 个 ULP，条件退化成 l <= r-1ULP，\n");
    printf("      一旦 l 与 r 相邻（差 1 ULP），该条件恒真，而 mid=(l+r)/2 又只能舍入成 l 或 r\n");
    printf("      → p(l) 为假时 l=mid=l 不前进 → 死循环。\n");
    return 0;
}
