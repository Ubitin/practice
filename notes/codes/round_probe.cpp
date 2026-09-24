// 检验 01.cpp 里 g() 的"四舍五入"到底对不对
// 参照：std::llround(x) —— 标准库的"四舍五入"（.5 一律远离 0），
//       以及"十进制精确四舍五入"（把 x 当十进制数，看十分位）
#include <bits/stdc++.h>
using namespace std;

// ===== 你写在 01.cpp 里的版本 =====
long long g_user(double x) {
    long long u = x;
    long long temp = x * 10;
    temp %= 10;
    if (temp < 5) return u;
    else if (temp >= 5) return u + 1;
}

int main() {
    printf("===== ① 网格扫描：x = k + d/10（k 从 -30 到 40）=====\n");
    long long bad = 0, total = 0;
    string firstPos, firstNeg;
    for (int k = -30; k <= 40; ++k)
        for (int d = 0; d <= 9; ++d) {
            double x = k + d / 10.0;
            long long a = g_user(x), b = llround(x);
            ++total;
            if (a != b) {
                ++bad;
                char buf[200];
                snprintf(buf, sizeof buf, "x=%.1f  你的 g=%lld  正确=%lld", x, a, b);
                if (x >= 0 && firstPos.empty()) firstPos = buf;
                if (x < 0 && firstNeg.empty()) firstNeg = buf;
            }
        }
    printf("  共 %lld 个用例，错 %lld 个（%.1f%%）\n", total, bad, 100.0 * bad / total);
    printf("  第一个非负反例：%s\n", firstPos.empty() ? "（无）" : firstPos.c_str());
    printf("  第一个负数反例：%s\n", firstNeg.empty() ? "（无）" : firstNeg.c_str());

    printf("\n===== ② 正数里的反例（含 .5 边界与浮点误差）=====\n");
    double xs[] = {0.5, 1.5, 2.5, 12.5, 0.49999999999999994, 2.675, 12.35, 100.5, 0.29, 3.7};
    for (double x : xs)
        printf("  x=%-22.17g 你的 g=%-4lld llround=%-4lld %s\n", x, g_user(x), llround(x),
               g_user(x) == llround(x) ? "OK" : "<<< 不一致");

    printf("\n===== ③ 用你自己公式算出来的 w 当输入（风寒指数）=====\n");
    printf("  t=温度  v=风速   w(未四舍五入)      你的 f()  llround  是否一致\n");
    long long badCnt = 0, shown = 0, cases = 0;
    for (long long t = -40; t <= 10; ++t)
        for (long long v = 1; v <= 120; ++v) {
            double w = 13.12 + 0.6215 * t - 11.37 * pow(v, 0.16) + 0.3665 * t * pow(v, 0.16);
            ++cases;
            long long a = g_user(w), b = llround(w);
            if (a != b) {
                ++badCnt;
                if (shown < 8) {
                    ++shown;
                    printf("  t=%-4lld v=%-4lld w=%-19.15g %-9lld %-9lld <<< 不一致\n", t, v, w, a, b);
                }
            }
        }
    printf("  扫描 %lld 组 (t,v)，不一致 %lld 组（%.2f%%）\n", cases, badCnt, 100.0 * badCnt / cases);
    return 0;
}
