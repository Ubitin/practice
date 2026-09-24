// 递归 DFS 的"深度上限"实测：链形树（最坏情况）上跑到多少层会栈溢出
// 用法: dfs_depth.exe <n>
#include <bits/stdc++.h>
using namespace std;

static const int MAXN = 300005;
vector<int> g[MAXN];
int cur = 0, mx = 0;

void dfs(int u) {
    ++cur;
    mx = max(mx, cur);
    for (int v : g[u]) dfs(v);
    --cur;
}

int main(int argc, char** argv) {
    int n = argc > 1 ? atoi(argv[1]) : 100000;
    for (int i = 2; i <= n; ++i) g[i - 1].push_back(i);   // 链：1-2-3-...-n
    dfs(1);
    printf("n=%d 完成，最大递归深度=%d\n", n, mx);
    return 0;
}
