// P1160 —— 数组模拟双向链表（哨兵 0 版，与 std::list 暴力版实测一致）
// 关键点：
//   ① 哨兵节点 0：a[0].nxt 指向最左节点，a[0].pre 指向最右节点
//   ② indexx[编号] = 该编号所在节点的数组下标 ⇒ 定位 O(1)
//   ③ del 必须【同时】改两侧：a[le].nxt = rt;  a[rt].pre = le;
//   ④ 输出从 a[0].nxt 开始往右走（这样"插到编号1左边"的节点也能打到）
#include <bits/stdc++.h>
using namespace std;

const int maxn = 100005;
struct node { int pre, nxt, key; };
node a[maxn];
int tot = 0;
int indexx[maxn];        // 编号 → 节点下标
bool dead[maxn];

void ins_back(int x, int y) {          // 把 y 插到 x 后面
    int now = indexx[x], t = ++tot;
    a[t] = {now, a[now].nxt, y};
    a[a[now].nxt].pre = t;
    a[now].nxt = t;
    indexx[y] = t;
}
void ins_front(int x, int y) {         // 把 y 插到 x 前面
    int now = indexx[x], t = ++tot;
    a[t] = {a[now].pre, now, y};
    a[a[now].pre].nxt = t;             // ★ 与 ins_back 对称的一行
    a[now].pre = t;
    indexx[y] = t;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    // 哨兵 0 自环（空链表）：nxt 与 pre 都指自己
    a[0] = {0, 0, 0};
    if (n >= 1) {
        indexx[1] = ++tot;
        a[tot] = {0, 0, 1};            // 第 1 个人：pre=nxt=0（即以哨兵为前后）
        a[0].nxt = tot;                // 哨兵 → 最左
        a[0].pre = tot;                // 哨兵 → 最右
    }
    for (int i = 2; i <= n; ++i) {
        int k, p; cin >> k >> p;
        if (p == 0) ins_front(k, i); else ins_back(k, i);
    }
    int m; cin >> m;
    while (m--) { int x; cin >> x; dead[x] = true; }   // 惰性删除：只打标记

    for (int now = a[0].nxt; now; now = a[now].nxt) {  // ★ 从最左开始
        if (!dead[a[now].key]) cout << a[now].key << " ";
    }
    cout << "\n";
    return 0;
}
