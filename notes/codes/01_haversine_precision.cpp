// ============================================================
//  01_haversine_precision.cpp —— Haversine 的四种写法精度对照
//
//  目的：把 01.cpp 的 WA 拆成"致命主因"和"精度隐患"两部分。
//        本程序专门隔离【数值精度】这一层（不涉及角度/弧度转换）。
//
//  构造：赤道上两点 (0,0) 与 (0,θ)，经度差 θ 弧度。
//        此时真值 d = R·θ（纯几何，不需要参考实现）。
//        四种写法都从 θ 出发，只有 hav() 形式与反解函数不同。
// ============================================================
#include <bits/stdc++.h>
using namespace std;

const double PI = acos(-1.0);
const double R  = 6371.0;

double hav_A(double x) { return (1.0 - cos(x)) / 2.0; }   // 原版 01.cpp 的写法
double hav_B(double x) { double s = sin(x / 2); return s * s; }  // 修正版写法

int main() {
    printf("经度差 theta 从 1e-1 递减到 1e-14（弧度），真值 d = R*theta\n");
    printf("表内是【相对误差】 |d_算 - d_真| / d_真\n\n");
    printf("%-12s %-14s %-14s %-14s %-14s\n",
           "theta", "V1 原版", "V2 只换hav", "V3 只换反解", "V4 修正版");
    printf("%-12s %-14s %-14s %-14s %-14s\n",
           "", "(1-cos)/2", "sin^2/2", "asin(sqrt)",
           "sin^2/2+asin");
    printf("%-12s %-14s %-14s %-14s %-14s\n",
           "", "+acos(1-2h)", "+acos(1-2h)", "+acos→换", "");
    printf("---------------------------------------------------------------"
           "-----------------\n");

    for (int k = 1; k <= 14; k++) {
        double th = pow(10.0, -k);
        double truth = R * th;

        double v1 = R * acos(1.0 - 2.0 * hav_A(th));            // 原版组合
        double v2 = R * acos(1.0 - 2.0 * hav_B(th));            // 仅改善 hav
        double v3 = R * 2.0 * asin(sqrt(hav_A(th)));            // 仅改善反解
        double v4 = R * 2.0 * asin(sqrt(hav_B(th)));            // 修正版组合

        auto rel = [&](double d) {
            double e = fabs(d - truth) / truth;
            if (e == 0.0) return string("< 1e-17");
            char buf[32]; snprintf(buf, sizeof buf, "%.2e", e); return string(buf);
        };
        printf("%-12.0e %-14s %-14s %-14s %-14s\n",
               th, rel(v1).c_str(), rel(v2).c_str(), rel(v3).c_str(), rel(v4).c_str());
    }

    printf("\n【结论】\n");
    printf("  1) 真正致命的是「角度没转弧度」——相对误差 51.5%%（见讲义），把三种精度\n");
    printf("     写法都试一遍也救不回来，本表里的差异在 4 位小数要求下根本看不出来；\n");
    printf("  2) 但三种旧写法在【两点非常接近】时会一起崩：theta <= 1e-8 时全部输出 0。\n");
    printf("     原因：\n");
    printf("       (1-cos x)/2  →  x 很小时 cos(x) 舍入成 1.0，1-1=0，h 直接变 0（相消）\n");
    printf("       acos(1-2h)   →  h 很小时 1-2h 舍入成 1.0，acos(1)=0（再一次相消）\n");
    printf("     只有 V4 同时避开了这两处相消：sin^2(x/2) 保住 h，asin(sqrt(h)) 保住 theta。\n");
    printf("  3) 所以正确写法是 V4 —— 即使这道题只改弧度就能 AC，也建议一次写对。\n");

    printf("\n【theta=1e-8 时的实际输出对比】（真值应为 %.10f km）\n", R * 1e-8);
    {
        double th = 1e-8;
        printf("  V1 (1-cos)/2 + acos(1-2h) : %.10f km  ← h 被算成 0\n",
               R * acos(1.0 - 2.0 * hav_A(th)));
        printf("     其中 hav_A(1e-8) = %.17g （真值 %.3e）\n", hav_A(th), th * th / 4);
        printf("  V2 sin^2/2  + acos(1-2h) : %.10f km  ← h 保住了，但 1-2h 又被舍入成 1\n",
               R * acos(1.0 - 2.0 * hav_B(th)));
        printf("     其中 hav_B(1e-8) = %.17g （正确！）\n", hav_B(th));
        printf("  V4 sin^2/2  + 2asin(sqrt): %.10f km  ✔\n", R * 2.0 * asin(sqrt(hav_B(th))));
    }

    // 演示样本点：三种精度写法在本例上都能过（说明主因不是精度）
    printf("\n【样本点回归】西安 -> 莫斯科（真值 5793.2236）\n");
    {
        double la1 = 34.260958, lo1 = 108.942369;
        double la2 = 55.755825, lo2 = 37.617298;
        double p1 = la1 * PI / 180, q1 = lo1 * PI / 180;
        double p2 = la2 * PI / 180, q2 = lo2 * PI / 180;
        double hA = hav_A(p2 - p1) + cos(p1) * cos(p2) * hav_A(q2 - q1);
        double hB = hav_B(p2 - p1) + cos(p1) * cos(p2) * hav_B(q2 - q1);
        printf("  原版组合 (1-cos)/2 + acos(1-2h) : %.10f km\n", R * acos(1.0 - 2.0 * hA));
        printf("  修正版组合 sin^2/2 + 2asin(sqrt): %.10f km\n", R * 2.0 * asin(sqrt(hB)));
        printf("  → 两者都四舍五入到 5793.2236；所以【只改弧度就能 AC】。\n");
    }
    return 0;
}
