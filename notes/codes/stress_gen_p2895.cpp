// P2895 随机数据生成器
// 用法: stress_gen_p2895.exe <seed> [maxM] [maxR] [maxT] [forceT0]
//   maxM: 陨石个数上限(默认10)   maxR: 坐标上限(默认8)   maxT: 时刻上限(默认9)
//   forceT0: 1 表示必定生成一颗 t=0 的陨石（专攻“起点被 0 时刻砸中”的边界）
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char** argv) {
    long long seed = argc > 1 ? atoll(argv[1]) : 1;
    int maxM = argc > 2 ? atoi(argv[2]) : 10;
    int maxR = argc > 3 ? atoi(argv[3]) : 8;
    int maxT = argc > 4 ? atoi(argv[4]) : 9;
    int forceT0 = argc > 5 ? atoi(argv[5]) : 0;
    mt19937_64 rng((unsigned long long)seed * 1000003ULL + 12345ULL);
    auto rnd = [&](int hi) { return (int)(rng() % (unsigned long long)(hi + 1)); };

    int M = 1 + rnd(maxM - 1);
    printf("%d\n", M);
    for (int i = 0; i < M; ++i) {
        int x = rnd(maxR), y = rnd(maxR);
        int t = rnd(maxT);
        if (forceT0 && i == 0) {
            t = 0;
            x = rnd(2);  // 砸在起点附近，保证原点 0 时刻就被摧毁
            y = 0;
        }
        printf("%d %d %d\n", x, y, t);
    }
    return 0;
}
