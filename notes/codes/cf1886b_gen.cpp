// CF1886B 随机数据生成器（种子从命令行来，风格轮换）
// 用法: cf1886b_gen.exe <seed>
//   0 一般随机      1 只在 {0,±1,±2} 附近（专测"答案很小"的边界）
//   2 点重合/对称   3 大坐标
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    long long seed = argc > 1 ? atoll(argv[1]) : 1;
    mt19937_64 rng((unsigned long long)seed * 1000003ULL + 23ULL);
    auto R = [&](int lo, int hi) { return (int)(lo + rng() % (unsigned long long)(hi - lo + 1)); };
    int style = (int)(seed % 4);

    int T = 1 + (int)(rng() % 3);
    printf("%d\n", T);
    for (int tc = 0; tc < T; ++tc) {
        int px, py, ax, ay, bx, by;
        if (style == 0) {
            px = R(-10, 10); py = R(-10, 10);
            ax = R(-10, 10); ay = R(-10, 10);
            bx = R(-10, 10); by = R(-10, 10);
        } else if (style == 1) {
            px = R(0, 2); py = R(0, 2);
            ax = R(0, 2); ay = R(0, 2);
            bx = R(0, 2); by = R(0, 2);
        } else if (style == 2) {
            // 让点大量重合：P=A、O=B、P=O 等边界
            ax = R(-3, 3); ay = R(-3, 3);
            px = (rng() % 2) ? ax : R(-3, 3);
            py = (rng() % 2) ? ay : R(-3, 3);
            bx = (rng() % 3 == 0) ? 0 : R(-3, 3);
            by = (rng() % 3 == 0) ? 0 : R(-3, 3);
        } else {
            px = R(-1000, 1000); py = R(-1000, 1000);
            ax = R(-1000, 1000); ay = R(-1000, 1000);
            bx = R(-1000, 1000); by = R(-1000, 1000);
        }
        printf("%d %d %d %d %d %d\n", px, py, ax, ay, bx, by);
    }
    return 0;
}
