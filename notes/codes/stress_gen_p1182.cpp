// ============================================================
//  stress_gen_p1182.cpp —— P1182 数据生成器（对拍用）
//
//  用法： stress_gen_p1182.exe <seed>
//  配套： 讲义/如何写对拍程序.md
//  seed % 5 决定数据风格 → 保证边界结构一定会被造到
// ============================================================
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    unsigned seed = (argc > 1) ? (unsigned)stoul(argv[1])
                               : (unsigned)chrono::steady_clock::now().time_since_epoch().count();
    mt19937 rng(seed);

    int style = (int)(seed % 5);
    int n = 1 + (int)(rng() % 12);      // 规模小，暴力 DP 才跑得动
    int m = 1 + (int)(rng() % n);       // 1 <= m <= n

    vector<long long> a(n + 1);
    switch (style) {
        case 0:                                            // 纯随机
            for (int i = 1; i <= n; ++i) a[i] = 1 + rng() % 20;
            break;
        case 1:                                            // 全部相同
            { long long v = 1 + rng() % 20; for (int i = 1; i <= n; ++i) a[i] = v; }
            break;
        case 2:                                            // 严格递增
            for (int i = 1; i <= n; ++i) a[i] = i;
            break;
        case 3:                                            // 一个超大 + 一堆 1（不均衡）
            for (int i = 1; i <= n; ++i) a[i] = 1;
            a[1 + (int)(rng() % n)] = 100 + (long long)(rng() % 100);
            break;
        case 4:                                            // m = n（每数一段）
            m = n;
            for (int i = 1; i <= n; ++i) a[i] = 1 + rng() % 20;
            break;
    }
    if (style == 0 || style == 1 || style == 2 || style == 3) {
        if (rng() % 4 == 0) m = 1;                         // 偶尔强制 m=1
    }

    printf("%d %d\n", n, m);
    for (int i = 1; i <= n; ++i) printf("%lld%c", a[i], i == n ? '\n' : ' ');
    return 0;
}
