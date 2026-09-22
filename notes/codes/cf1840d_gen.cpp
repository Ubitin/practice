// CF1840D 随机数据生成器（种子从命令行来，风格轮换）
// 用法: cf1840d_gen.exe <seed>
//   0 纯随机：n 1..8，值域 0..20
//   1 大量重复：只有 1~3 种取值（答案往往是 0）
//   2 两个"团"：值聚成两堆且相距很远（专门逼出"要不要用第 3 个工匠"的分界）
//   3 跨度极大 + 少点：如 {0, 1, 100, 101} 这种"整数型号"与"实数型号"会产生分歧的结构
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    long long seed = argc > 1 ? atoll(argv[1]) : 1;
    mt19937_64 rng((unsigned long long)seed * 1000003ULL + 13ULL);
    auto rnd = [&](int hi) { return (int)(rng() % (unsigned long long)(hi + 1)); };
    int style = (int)(seed % 4);

    int T = 1 + rnd(4);
    printf("%d\n", T);
    for (int tc = 0; tc < T; ++tc) {
        int n, maxV;
        if (style == 0) { n = 1 + rnd(7); maxV = 20; }
        else if (style == 1) { n = 1 + rnd(7); maxV = 2; }
        else if (style == 2) { n = 3 + rnd(5); maxV = 8; }
        else { n = 2 + rnd(3); maxV = 6; }
        printf("%d\n", n);
        vector<int> v;
        for (int i = 0; i < n; ++i) {
            int x;
            if (style == 1) x = rnd(maxV);
            else if (style == 2) x = (i < (n + 1) / 2 ? rnd(maxV) : 50 + rnd(maxV));
            else if (style == 3) x = (rnd(1) ? rnd(maxV) : 100 + rnd(maxV));
            else x = rnd(maxV);
            v.push_back(x);
        }
        for (int i = 0; i < n; ++i) printf("%d%c", v[i], i + 1 == n ? '\n' : ' ');
    }
    return 0;
}
