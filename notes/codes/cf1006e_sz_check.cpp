// 验证一件事：因为题目保证 p_i < i，所以"子树大小"根本不用 DFS，
// 只要【按编号从大到小扫一遍】把大小加给父亲即可。
// 这里把两种算法在大量随机树上逐点比较，必须完全一致。
#include <bits/stdc++.h>
using namespace std;

int n;
vector<int> ch[64], par;
int szDfs[64], szLoop[64];

void dfs(int u) {
    szDfs[u] = 1;
    for (int v : ch[u]) { dfs(v); szDfs[u] += szDfs[v]; }
}

int main() {
    mt19937_64 rng(777ULL);
    long long trees = 0, bad = 0;
    for (int tc = 0; tc < 20000; ++tc) {
        n = 1 + (int)(rng() % 20);
        for (int i = 1; i <= n; ++i) ch[i].clear();
        par.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i) {
            int style = (int)(rng() % 3);
            int p = (style == 0) ? 1 : (style == 1 ? i - 1 : 1 + (int)(rng() % (i - 1)));
            par[i] = p;                 // 保证 p < i
            ch[p].push_back(i);
        }
        dfs(1);                                          // 算法①：真的 DFS
        for (int i = 1; i <= n; ++i) szLoop[i] = 1;      // 算法②：倒着扫编号
        for (int i = n; i >= 2; --i) szLoop[par[i]] += szLoop[i];

        ++trees;
        for (int i = 1; i <= n; ++i)
            if (szDfs[i] != szLoop[i]) { ++bad; break; }
    }
    printf("随机树 %lld 棵\n", trees);
    printf("  DFS 算的子树大小 vs 倒着扫编号算的子树大小：不一致 %lld 棵\n", bad);
    printf("结论：因为 p_i < i，【倒着按编号扫一遍】%s\n", bad == 0 ? "完全等价 ✅" : "不等价 ✘");
    return 0;
}
