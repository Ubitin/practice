// CF1006E 的"真实 DFS"（要算 tin / sz / ord）在链形树上能递归多少层？
// 用法: cf1006e_dfs_probe.exe <n>
#include <bits/stdc++.h>
using namespace std;

static const int MAXN = 300005;
vector<int> ch[MAXN];      // 孩子表
int tin_[MAXN], sz_[MAXN], ord_[MAXN];
int timer_ = 0, maxdep = 0;

void dfs(int u, int dep) {
    maxdep = max(maxdep, dep);
    tin_[u] = ++timer_;
    ord_[timer_] = u;
    sz_[u] = 1;
    for (int v : ch[u]) {
        dfs(v, dep + 1);
        sz_[u] += sz_[v];
    }
}

int main(int argc, char** argv) {
    int n = argc > 1 ? atoi(argv[1]) : 100000;
    for (int i = 2; i <= n; ++i) ch[i - 1].push_back(i);   // 链：1-2-3-...-n
    dfs(1, 1);
    // 顺便验一下"子树 = 连续区间"：链上子树(u) = [tin[u], tin[u]+sz[u]-1]
    bool ok = true;
    for (int u = 1; u <= n; ++u)
        for (int k = 1; k <= sz_[u]; ++k)
            if (ord_[tin_[u] + k - 1] != u + k - 1) { ok = false; break; }
    printf("n=%d 完成，最大递归深度=%d，子树连续区间校验=%s\n", n, maxdep, ok ? "通过" : "失败");
    return 0;
}
