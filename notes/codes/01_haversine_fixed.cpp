// ============================================================
//  01_haversine_fixed.cpp —— Haversine 球面距离（修正版）
//
//  原文件 test_/01.cpp 保持原样、未做任何改动。
//
//  相对原版的三处修正：
//   ①【致命主因】输入是【角度】，而 sin/cos 吃【弧度】
//       → 必须先 * PI/180 把 4 个量全部转成弧度
//   ② hav(x) 改用 sin(x/2)*sin(x/2)
//       （与 (1-cos x)/2 数学等价，但小角度时没有相消误差）
//   ③ 反解用 2*asin(sqrt(h))，而不是 acos(1-2h)
//       （小距离时后者要从接近 1 的值反解，精度差；见讲义）
//  其余（读入方式、r=6371、输出格式 %.4fkm）与原版一致。
// ============================================================
#include <bits/stdc++.h>
using namespace std;

const double PI = acos(-1.0);   // 最稳的 pi（M_PI 在 -std=c++17 下未定义）
const double R  = 6371.0;       // 地球半径 km

double hav(double x) {          // 半正矢 hav(x) = sin^2(x/2) = (1-cos x)/2
    double s = sin(x / 2);
    return s * s;               // 用 sin^2 形式，小角度不丢精度
}

int main() {
    double lat1, lon1, lat2, lon2;
    cin >> lat1 >> lon1 >> lat2 >> lon2;

    // ★ 原代码缺的就是这一步：角度 → 弧度
    lat1 *= PI / 180.0;
    lon1 *= PI / 180.0;
    lat2 *= PI / 180.0;
    lon2 *= PI / 180.0;

    double h = hav(lat2 - lat1)
             + cos(lat1) * cos(lat2) * hav(lon2 - lon1);

    if (h > 1.0) h = 1.0;       // 兜底：浮点误差可能让 h 略微超过 1
    if (h < 0.0) h = 0.0;

    double theta = 2.0 * asin(sqrt(h));   // 圆心角（弧度）
    printf("%.4fkm\n", R * theta);
    return 0;
}
