// ============================================================
//  stress_gen.cpp —— 对拍数据生成器【模板】
//
//  用法：gen.exe <seed>
//    · seed 由对拍器传入 → 每次跑的数据不同，但同 seed 完全可复现
//    · seed % 4 决定"数据风格"，保证边界数据一定会被覆盖到
//
//  配套讲义：讲义/如何写对拍程序.md
//  本文件以 CF1486D（长度≥k 子段的最大中位数）为例，换题只改 genCase() 即可。
// ============================================================
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    unsigned seed = (argc > 1) ? (unsigned)stoul(argv[1])
                               : (unsigned)chrono::steady_clock::now().time_since_epoch().count();
    mt19937 rng(seed);

    int style = (int)(seed % 4);     // ★ 四种风格轮着来，专治"随机数据测不出边界 bug"

    int n = 1 + (int)(rng() % 12);   // 规模小：暴力解才跑得动
    int k = 1 + (int)(rng() % n);    // 1 <= k <= n

    vector<int> a(n);
    switch (style) {
        case 0:                                   // 纯随机
            for (auto& x : a) x = 1 + (int)(rng() % n);
            break;
        case 1:                                   // 全部相同（最容易被忽略的边界）
            { int v = 1 + (int)(rng() % n); for (auto& x : a) x = v; }
            break;
        case 2:                                   // 严格递增（让"全局次序统计量"类错法蒙对）
            for (int i = 0; i < n; ++i) a[i] = i + 1;
            break;
        case 3:                                   // 只有两种值（大量重复）
            for (auto& x : a) x = (rng() % 2) ? 1 : n;
            break;
    }

    // 输出格式必须与题目完全一致
    printf("%d %d\n", n, k);
    for (int i = 0; i < n; ++i) printf("%d%c", a[i], i + 1 == n ? '\n' : ' ');
    return 0;
}
