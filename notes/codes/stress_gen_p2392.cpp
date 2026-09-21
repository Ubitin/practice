// ============================================================
//  stress_gen_p2392.cpp —— P2392 数据生成器（对拍用）
//
//  用法： stress_gen_p2392.exe <seed>
//  输出： 第一行 s1 s2 s3 s4，随后 4 行每行的题目用时
//  seed % 4 决定风格，保证边界结构一定被造到
// ============================================================
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    unsigned seed = (argc > 1) ? (unsigned)stoul(argv[1])
                               : (unsigned)chrono::steady_clock::now().time_since_epoch().count();
    mt19937 rng(seed);
    int style = (int)(seed % 4);

    int mx = 10;                                  // 规模小：暴力 DFS 才跑得动
    int s[4];
    for (int i = 0; i < 4; ++i) s[i] = 1 + (int)(rng() % mx);

    printf("%d %d %d %d\n", s[0], s[1], s[2], s[3]);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < s[i]; ++j) {
            int v;
            switch (style) {
                case 0: v = 1 + (int)(rng() % 20);   break;   // 纯随机
                case 1: v = 5;                       break;   // 全部相同
                case 2: v = j + 1;                   break;   // 递增（1,2,3,…）
                case 3: v = (rng() % 2) ? 1 : 20;    break;   // 只有两种值（极端不均衡）
            }
            printf("%d%c", v, j + 1 == s[i] ? '\n' : ' ');
        }
    }
    return 0;
}
