// CF1006E —— 直接按题面模拟的暴力解（对每个查询从 u 现场跑一遍传递顺序）
// 用途：小数据下当"答案核对器"。O(nq)，绝对不能提交。
#include <bits/stdc++.h>
using namespace std;

vector<int> ch[200005];
int cnt, foundNode;

void dfs(int u, int k) {          // 按题面：自己先收到，再依次传给每个下属
    if (foundNode != -1) return;
    ++cnt;
    if (cnt == k) { foundNode = u; return; }
    for (int v : ch[u]) {
        dfs(v, k);
        if (foundNode != -1) return;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    if (!(cin >> n >> q)) return 0;
    for (int i = 2; i <= n; ++i) {
        int p;
        cin >> p;
        ch[p].push_back(i);       // i 从小到大读入 ⇒ 孩子表天然是编号升序 = 题面的"名单顺序"
    }
    while (q--) {
        int u, k;
        cin >> u >> k;
        cnt = 0;
        foundNode = -1;
        dfs(u, k);
        cout << foundNode << "\n";   // 不够 k 个时 foundNode 仍是 -1
    }
    return 0;
}
