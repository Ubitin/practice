// CF1840D 极限性能测试数据生成器：cf1840d_gen_big.exe <seed> [n] [maxV]
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char** argv) {
    long long seed = argc > 1 ? atoll(argv[1]) : 1;
    int n = argc > 2 ? atoi(argv[2]) : 200000;
    long long maxV = argc > 3 ? atoll(argv[3]) : 1000000000LL;
    mt19937_64 rng((unsigned long long)seed * 1000003ULL + 17ULL);
    printf("1\n%d\n", n);
    for (int i = 0; i < n; ++i)
        printf("%lld%c", (long long)(rng() % (unsigned long long)(maxV + 1)), i + 1 == n ? '\n' : ' ');
    return 0;
}
