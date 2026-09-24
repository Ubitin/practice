// CF1886B 大批量数据生成器：一次生成 T 组测试（混合 4 种风格）
// 用法: cf1886b_biggen.exe <seed> <T>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    long long seed = argc > 1 ? atoll(argv[1]) : 1;
    int T = argc > 2 ? atoi(argv[2]) : 5000;
    mt19937_64 rng((unsigned long long)seed * 1000003ULL + 29ULL);
    auto R = [&](int lo, int hi) { return (int)(lo + rng() % (unsigned long long)(hi - lo + 1)); };

    printf("%d\n", T);
    for (int tc = 0; tc < T; ++tc) {
        int style = (int)(rng() % 4);
        int px, py, ax, ay, bx, by;
        if (style == 0) {                       // 一般随机
            px = R(-10, 10); py = R(-10, 10);
            ax = R(-10, 10); ay = R(-10, 10);
            bx = R(-10, 10); by = R(-10, 10);
        } else if (style == 1) {                // 值都很小（答案可能 < 0.1）
            px = R(0, 2); py = R(0, 2);
            ax = R(0, 2); ay = R(0, 2);
            bx = R(0, 2); by = R(0, 2);
        } else if (style == 2) {                // 大量重合（P=A、O=B）
            ax = R(-3, 3); ay = R(-3, 3);
            px = (rng() % 2) ? ax : R(-3, 3);
            py = (rng() % 2) ? ay : R(-3, 3);
            bx = (rng() % 3 == 0) ? 0 : R(-3, 3);
            by = (rng() % 3 == 0) ? 0 : R(-3, 3);
        } else {                                // 大坐标
            px = R(-1000, 1000); py = R(-1000, 1000);
            ax = R(-1000, 1000); ay = R(-1000, 1000);
            bx = R(-1000, 1000); by = R(-1000, 1000);
        }
        printf("%d %d %d %d %d %d\n", px, py, ax, ay, bx, by);
    }
    return 0;
}
