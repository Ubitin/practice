// 造一个 500×500 的"蛇形走廊"：空格连成一条长蛇，DFS 深度 ≈ 12.5 万，专测递归爆栈
// 用法: cf377a_snake.exe <n> [m]   （输出 n m k 与网格）
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char** argv) {
    int n = argc > 1 ? atoi(argv[1]) : 500;
    int m = argc > 2 ? atoi(argv[2]) : 500;
    vector<string> g(n, string(m, '#'));
    // 偶数行整行挖通；奇数行只在左右两端挖通，形成蛇形
    for (int i = 0; i < n; i += 2)
        for (int j = 0; j < m; ++j) g[i][j] = '.';
    for (int i = 1; i < n; i += 2)
        g[i][(i % 4 == 1) ? m - 1 : 0] = '.';
    int E = 0; for (auto& r : g) E += (int)count(r.begin(), r.end(), '.');
    printf("%d %d %d\n", n, m, 5);            // 随便删 5 个，只看深度
    for (auto& r : g) printf("%s\n", r.c_str());
    fprintf(stderr, "空格数 = %d（DFS 深度可达这个量级）\n", E);
    return 0;
}
