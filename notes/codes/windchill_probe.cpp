// 样例 120 35 -> 40 到底是哪个公式算出来的？
#include <bits/stdc++.h>
using namespace std;

double W(double t, double v) {           // 你代码里的公式（标准风寒/寒意指数）
    return 13.12 + 0.6215 * t - 11.37 * pow(v, 0.16) + 0.3665 * t * pow(v, 0.16);
}

int main() {
    double V = 120, T = 35;
    printf("=== ① 按你代码里的公式，把样例算到小数点后 6 位 ===\n");
    printf("   t=%.0f  v=%.0f\n", T, V);
    printf("   v^0.16            = %.10f\n", pow(V, 0.16));
    printf("   13.12             = %.4f\n", 13.12);
    printf("   0.6215*t          = %.4f\n", 0.6215 * T);
    printf("  -11.37*v^0.16      = %.4f\n", -11.37 * pow(V, 0.16));
    printf("   0.3665*t*v^0.16   = %.4f\n", 0.3665 * T * pow(V, 0.16));
    printf("   ----------------------------\n");
    printf("   合计 W            = %.6f   → 四舍五入 = %.0f\n", W(T, V), llround(W(T, V)));
    printf("   输入反过来(v=35,t=120)      = %.4f → %.0f\n", W(120, 35), llround(W(120, 35)));

    printf("\n=== ② 想让样例变成 40，需要改哪里？（逐个试）===\n");
    printf("   把首项 13.12 改成 15.12      ：W = %.4f  → %.0f\n",
           15.12 + 0.6215 * T - 11.37 * pow(V, 0.16) + 0.3665 * T * pow(V, 0.16),
           llround(15.12 + 0.6215 * T - 11.37 * pow(V, 0.16) + 0.3665 * T * pow(V, 0.16)));
    printf("   幂作用在 (v*t) 上            ：W = %.4f  → %.0f\n",
           [&] { double x = pow(V * T, 0.16); return 13.12 + 0.6215 * T - 11.37 * x + 0.3665 * T * x; }(),
           llround([&] { double x = pow(V * T, 0.16); return 13.12 + 0.6215 * T - 11.37 * x + 0.3665 * T * x; }()));
    {
        double x = pow(V / 3.6, 0.16);      // v 用 m/s
        printf("   v 先换算成 m/s               ：W = %.4f  → %.0f\n",
               13.12 + 0.6215 * T - 11.37 * x + 0.3665 * T * x,
               llround(13.12 + 0.6215 * T - 11.37 * x + 0.3665 * T * x));
    }
    for (double e : {0.16, 0.24, 0.28, 0.32}) {
        double x = pow(V, e), w = 13.12 + 0.6215 * T - 11.37 * x + 0.3665 * T * x;
        printf("   指数改成 %.2f               ：W = %.4f  → %.0f\n", e, w, llround(w));
    }
    printf("   首项改成 13.12+2*0.6215? 之类的都不标准，略\n");

    printf("\n=== ③ 用标准公式，哪些整数 (v,t) 会得到 40？（v=120 附近）===\n");
    int cnt = 0;
    for (int v = 1; v <= 400 && cnt < 12; ++v) {
        for (int t = -60; t <= 50; ++t) {
            if (llround(W(t, v)) == 40) {
                printf("   v=%-4d t=%-4d W=%.4f\n", v, t, W(t, v));
                if (++cnt >= 12) break;
            }
        }
    }
    printf("\n   固定 v=120 时，能凑出 40 的 t = ");
    for (int t = -60; t <= 60; ++t) if (llround(W(t, 120)) == 40) printf("%d ", t);
    printf("\n");
    return 0;
}
