// CF1886B Fear of the Dark —— 参考解（标准四情况闭式公式）
// 令 O=(0,0)，P 为起点，A、B 为两盏灯的位置，w 为灯的照明半径。要求 w 最小，使得 O 与 P
// 落在两个半径 w 的圆盘的并集里的同一个连通块中。只有四种可能：
//   ① 两盏灯都用不上，A 一盏同时罩住 O 和 P：w ≥ max(|OA|, |PA|)
//   ② B 一盏同时罩住 O 和 P        ：w ≥ max(|OB|, |PB|)
//   ③ O 靠 A、P 靠 B，且两圆相交（能走过去）：w ≥ max(|AB|/2, |OA|, |PB|)
//   ④ O 靠 B、P 靠 A，同理          ：w ≥ max(|AB|/2, |OB|, |PA|)
// 答案 = 四种情况的最小值。
#include <bits/stdc++.h>
using namespace std;

double d(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        double px, py, ax, ay, bx, by;
        scanf("%lf %lf %lf %lf %lf %lf", &px, &py, &ax, &ay, &bx, &by);
        double OA = d(0, 0, ax, ay), OB = d(0, 0, bx, by);
        double PA = d(px, py, ax, ay), PB = d(px, py, bx, by);
        double AB = d(ax, ay, bx, by);
        double ans = min(min(max(OA, PA), max(OB, PB)),
                         min(max(max(AB / 2, OA), PB), max(max(AB / 2, OB), PA)));
        printf("%.10f\n", ans);
    }
    return 0;
}
