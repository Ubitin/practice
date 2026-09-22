// 大数据生成器：stress_gen_big2895.exe <seed> [maxCoord] [M] [maxT]
// 默认 maxCoord=300, M=50000, maxT=1000（按题面）
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char** argv) {
    long long seed = argc > 1 ? atoll(argv[1]) : 1;
    int maxCoord = argc > 2 ? atoi(argv[2]) : 300;
    int M = argc > 3 ? atoi(argv[3]) : 50000;
    int maxT = argc > 4 ? atoi(argv[4]) : 1000;
    mt19937_64 rng((unsigned long long)seed * 1000003ULL + 7ULL);
    auto rnd = [&](int hi) { return (int)(rng() % (unsigned long long)(hi + 1)); };
    printf("%d\n", M);
    for (int i = 0; i < M; ++i) printf("%d %d %d\n", rnd(maxCoord), rnd(maxCoord), rnd(maxT));
    return 0;
}
