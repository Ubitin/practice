// 反推：公式里每个量要改成多少，(v=120,t=35) 才会四舍五入成 40？
#include <bits/stdc++.h>
using namespace std;

double W(double t, double v) { return 13.12 + 0.6215 * t - 11.37 * pow(v, 0.16) + 0.3665 * t * pow(v, 0.16); }

int main() {
    double V = 120, T = 35;
    double vp = pow(V, 0.16);
    printf("=== ① 你代码里公式的结果 ===\n");
    printf("   W(35,120) = %.6f  → 四舍五入 = %lld\n", W(T, V), llround(W(T, V)));
    printf("   （反过来 t=120,v=35：W = %.4f → %lld）\n", W(120, 35), llround(W(120, 35)));

    printf("\n=== ② 固定 v=120，用标准公式能不能找到整数 t 得到 40？ ===\n");
    int found = 0;
    for (int t = -100; t <= 100; ++t) if (llround(W(t, 120)) == 40) { printf("   t=%d\n", t); ++found; }
    printf("   共 %d 个整数 t。\n", found);

    printf("\n=== ③ 反推：每个量要落在什么范围，样例才是 40（即 W∈[39.5,40.5)）===\n");
    // 首项常数 c
    {
        double lo = 39.5 - (0.6215 * T - 11.37 * vp + 0.3665 * T * vp), hi = 40.5 - (0.6215 * T - 11.37 * vp + 0.3665 * T * vp);
        printf("   首项常数 c：需要 c ∈ [%.4f, %.4f)   ← 若题面是 15.12 → W=%.4f ✓\n", lo, hi,
               15.12 + 0.6215 * T - 11.37 * vp + 0.3665 * T * vp);
    }
    {
        double base = 13.12 - 11.37 * vp;
        double lo = (39.5 - base) / (T + T * vp), hi = (40.5 - base) / (T + T * vp);
        printf("   t 的系数 a（且 t·v^0.16 的系数按同比例？不成立）: 单独改 a 需要 a ∈ [%.4f, %.4f)\n", lo, hi);
    }
    {
        double base = 13.12 + 0.6215 * T;
        double lo = (base + 0.3665 * T * vp - 40.5) / vp, hi = (base + 0.3665 * T * vp - 39.5) / vp;
        printf("   v^0.16 的系数 b：需要 b ∈ (%.4f, %.4f]   ← 旧风寒公式里有 10.45\n", lo, hi);
    }
    {
        double base = 13.12 + 0.6215 * T - 11.37 * vp;
        double lo = (39.5 - base) / (T * vp), hi = (40.5 - base) / (T * vp);
        printf("   t·v^0.16 的系数 d：需要 d ∈ [%.4f, %.4f)   ← 0.3965 落在里面 ✓\n", lo, hi);
    }
    {
        // 指数 e 使 W=39.5..40.5
        double lo = 0, hi = 0;
        for (double e = 0.0; e <= 1.0; e += 1e-6) {
            double x = pow(V, e), w = 13.12 + 0.6215 * T - 11.37 * x + 0.3665 * T * x;
            if (w >= 39.5 && w <= 40.5) { if (lo == 0) lo = e; hi = e; }
        }
        printf("   指数 e：需要 e ∈ [%.4f, %.4f]   ← 0.16 远在范围外；0.28 落在里面\n", lo, hi);
    }
    printf("\n=== ④ 几个「看着像抄错」的版本实测 ===\n");
    printf("   首项 15.12          → W = %.4f → %lld\n",
           15.12 + 0.6215 * T - 11.37 * vp + 0.3665 * T * vp, llround(15.12 + 0.6215 * T - 11.37 * vp + 0.3665 * T * vp));
    printf("   末项系数 0.3965     → W = %.4f → %lld\n",
           13.12 + 0.6215 * T - 11.37 * vp + 0.3965 * T * vp, llround(13.12 + 0.6215 * T - 11.37 * vp + 0.3965 * T * vp));
    printf("   v^0.16 系数 10.45   → W = %.4f → %lld\n",
           13.12 + 0.6215 * T - 10.45 * vp + 0.3665 * T * vp, llround(13.12 + 0.6215 * T - 10.45 * vp + 0.3665 * T * vp));
    printf("   幂作用在 v*t 上     → W = %.4f → %lld\n",
           [&] { double x = pow(V * T, 0.16); return 13.12 + 0.6215 * T - 11.37 * x + 0.3665 * T * x; }(),
           llround([&] { double x = pow(V * T, 0.16); return 13.12 + 0.6215 * T - 11.37 * x + 0.3665 * T * x; }()));
    printf("   指数 0.28           → W = %.4f → %lld\n",
           [&] { double x = pow(V, 0.28); return 13.12 + 0.6215 * T - 11.37 * x + 0.3665 * T * x; }(),
           llround([&] { double x = pow(V, 0.28); return 13.12 + 0.6215 * T - 11.37 * x + 0.3665 * T * x; }()));
    return 0;
}
