// CF1006E 随机数据生成器（种子从命令行来，风格轮换）
// 用法: cf1006e_gen.exe <seed>
//   0 随机树      1 链          2 菊花（全挂在 1 下）      3 毛毛虫（半链半挂）
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    long long seed = argc > 1 ? atoll(argv[1]) : 1;
    mt19937_64 rng((unsigned long long)seed * 1000003ULL + 19ULL);
    auto rnd = [&](int hi) { return (int)(rng() % (unsigned long long)(hi + 1)); };
    int style = (int)(seed % 4);

    int n = 1 + rnd(11);                 // 1..12，小到暴力解能跑
    int q = 1 + rnd(7);
    vector<int> p(n + 1, 1);
    for (int i = 2; i <= n; ++i) {
        if (style == 1)      p[i] = i - 1;          // 链
        else if (style == 2) p[i] = 1;              // 菊花
        else if (style == 3) p[i] = (i % 2 == 0 ? max(1, i - 1) : max(1, i - 2));
        else                 p[i] = 1 + rnd(i - 2 < 0 ? 0 : i - 2);  // 随机（保证 p<i）
        if (p[i] < 1) p[i] = 1;
        if (p[i] >= i) p[i] = i - 1 >= 1 ? i - 1 : 1;
    }
    printf("%d %d\n", n, q);
    for (int i = 2; i <= n; ++i) printf("%d%c", p[i], i == n ? '\n' : ' ');
    if (n == 1) printf("\n");
    for (int i = 0; i < q; ++i) {
        int u = 1 + rnd(n - 1);
        int k = 1 + rnd(n + 1);          // 故意越界，测 -1 分支
        printf("%d %d\n", u, k);
    }
    return 0;
}
