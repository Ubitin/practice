// P1160 队列安排 —— 暴力参考解（用 std::list + 迭代器数组，语义等价但实现完全不同）
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<list<int>::iterator> pos(n + 1);
    list<int> L;
    L.push_back(1);
    pos[1] = L.begin();
    for (int i = 2; i <= n; ++i) {
        int k, p; cin >> k >> p;
        auto it = pos[k];
        if (p == 0) pos[i] = L.insert(it, i);       // 插在 k 左边
        else        pos[i] = L.insert(next(it), i); // 插在 k 右边
    }
    int m; cin >> m;
    vector<bool> dead(n + 1, false);
    while (m--) {
        int x; cin >> x;
        if (!dead[x]) { L.erase(pos[x]); dead[x] = true; }
    }
    for (int v : L) cout << v << " ";
    cout << "\n";
    return 0;
}
