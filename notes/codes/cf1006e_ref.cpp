// ⚠️⚠️ 这是【答案核对器 / 参考实现】—— 你要自己写的话，别打开这个文件。
//        （用法：把你自己的解编译成 my.exe，然后
//          stress_cmp.exe cf1006e_gen.exe my.exe cf1006e_ref.exe 500 3000）
//
// CF1006E Military Problem —— 标准解：一次 DFS 求先根序 + 子树大小
//
// 关键结论：一棵子树在"一次全局 DFS 的先根序"里占【一段连续区间】
//           子树(u) 占 ord[tin[u] .. tin[u] + sz[u] - 1]
// 于是查询 (u,k)：k > sz[u] → -1；否则答案 = ord[tin[u] + k - 1]（O(1)）
//
// 本实现用【显式栈的迭代版】，避免链形树（n 可达 2e5）把递归栈撑爆。
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    vector<vector<int>> ch(n + 1);
    vector<int> par(n + 1, 0);
    for (int i = 2; i <= n; ++i) {
        int p;
        cin >> p;
        par[i] = p;
        ch[p].push_back(i);       // i 递增读入 ⇒ 孩子表即题面的"名单顺序"
    }

    vector<int> tin(n + 1, 0), ord(n + 1, 0);
    vector<long long> sz(n + 1, 1);

    // ① 迭代版先根序遍历（显式栈）。孩子要【逆序】入栈，才能按名单顺序访问
    vector<int> st;
    st.push_back(1);
    int timer = 0;
    while (!st.empty()) {
        int u = st.back();
        st.pop_back();
        tin[u] = ++timer;
        ord[timer] = u;
        for (int i = (int)ch[u].size() - 1; i >= 0; --i) st.push_back(ch[u][i]);
    }

    // ② 子树大小：按【先根序的逆序】把大小累加给父亲（此时孩子的 sz 一定已算好）
    for (int i = timer; i >= 2; --i) {
        int u = ord[i];
        sz[par[u]] += sz[u];
    }

    while (q--) {
        int u, k;
        cin >> u >> k;
        if (k > sz[u]) cout << -1 << "\n";
        else cout << ord[tin[u] + k - 1] << "\n";
    }
    return 0;
}
