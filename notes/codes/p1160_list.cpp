// 教材例 15-12（洛谷 P1160 队列安排）—— 数组模拟双向链表 + indexx 索引
// 按图中代码的逻辑逐行复刻（细节：node 含 pre/next/key，indexx 记录每个编号的节点下标）
#include <bits/stdc++.h>
using namespace std;

const int maxn = 100005;
struct node {
    int pre, nxt, key;
    node(int _pre = 0, int _nxt = 0, int _key = 0) : pre(_pre), nxt(_nxt), key(_key) {}
};
node a[maxn];
int n, m, tot = 0;
int indexx[maxn];          // indexx[编号] = 该编号所在节点的数组下标

// 把编号 y 插到编号 x 所在节点的【后面】
void ins_back(int x, int y) {
    int now = indexx[x];
    int t = ++tot;
    a[t] = node(now, a[now].nxt, y);
    a[a[now].nxt].pre = t;
    a[now].nxt = t;
    indexx[y] = t;
}

// 把编号 y 插到编号 x 所在节点的【前面】
void ins_front(int x, int y) {
    int now = indexx[x];
    int t = ++tot;
    a[t] = node(a[now].pre, now, y);
    a[a[now].pre].nxt = t;
    a[now].pre = t;
    indexx[y] = t;
}

// 删除编号 x 所在的节点（教材版：真删，改指针）
void del(int x) {
    if (!indexx[x]) return;                 // 已删过
    int now = indexx[x];
    int le = a[now].pre, rt = a[now].nxt;
    a[le].nxt = rt;
    a[rt].pre = le;
    indexx[x] = 0;
}

int main() {
    if (!(cin >> n)) return 0;
    tot = 0;
    indexx[1] = ++tot;
    a[tot] = node(0, 0, 1);                  // 代码技巧：pre=0 当"最左边界"
    for (int i = 2; i <= n; ++i) {
        int k, p;
        cin >> k >> p;
        if (p == 0) ins_front(k, i);
        else        ins_back(k, i);
    }
    cin >> m;
    while (m--) {
        int x; cin >> x;
        del(x);
    }
    int now = a[0].nxt;                      // 从哨兵的 nxt 开始（即最左）——教材这里用 now=a[0].nxt
    while (now) {
        cout << a[now].key << " ";
        now = a[now].nxt;
    }
    cout << "\n";
    return 0;
}
