// RGB → HSV 修正版（保留你原来的骨架，问题处用 ★ 标出）
// 输入：R G B 的字节值（0..255）；输出：H,S%,V%，逗号间隔，保留 4 位小数
#include <bits/stdc++.h>
using namespace std;

int main() {
    double r, g, b;
    if (!(cin >> r >> g >> b)) return 0;

    r /= 255.0; g /= 255.0; b /= 255.0;          // ★1 归一化到 [0,1]：除以 255（不是 256），且必须用 double 除

    double M = max(max(r, g), max(g, b));
    double m = min(min(r, g), min(g, b));
    double d = M - m;                            // ★2 d 必须是 double！用 int 会变整除

    double V = M;
    double S = (M == 0.0) ? 0.0 : d / M;         // 题面：V=0 时 S=0（避免 0/0）
    double H = 0.0;                              // ★3 灰度（d==0）时 H 无定义，约定取 0（与 Python colorsys 一致）

    if (d != 0.0) {                              // ★4 不判 d==0 就会整数除零 → RE
        if (M == r)      H = 60.0 * (0.0 + (g - b) / d);
        else if (M == g) H = 60.0 * (2.0 + (b - r) / d);
        else             H = 60.0 * (4.0 + (r - g) / d);
        if (H < 0.0) H += 360.0;                 // ★5 题面明确要求：H<0 就加 360
    }

    printf("%.4f,%.4f%%,%.4f%%\n", H, S * 100.0, V * 100.0);   // ★6 百分号要写 %%；H 不带 %
    return 0;
}
