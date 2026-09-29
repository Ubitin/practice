// P1160 教材版（忠实复刻，打印从编号 1 的节点开始）
#include <bits/stdc++.h>
using namespace std;

const int maxn = 100005;
struct node {
    int pre, nxt, key;
    node(int _pre = 0, int _nxt = 0, int _key = 0) : pre(_pre), nxt(_nxt), key(_key) {}
};
node a[maxn];
int n, m, tot = 0;
int indexx[maxn];

void ins_back(int x, int y) {
    int now = indexx[x];
    int t = ++tot;
    a[t] = node(now, a[now].nxt, y);
    a[a[now].nxt].pre = t;
    a[now].nxt = t;
    indexx[y] = t;
}
void ins_front(int x, int y) {
    int now = indexx[x];
    int t = ++tot;
    a[t] = node(a[now].pre, now, y);
    a[a[now].pre].nxt = t;
    a[now].pre = t;
    indexx[y] = t;
}
void del(int x) {
    if (!indexx[x]) return;
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
    a[tot] = node(0, 0, 1);
    for (int i = 2; i <= n; ++i) {
        int k, p; cin >> k >> p;
        if (p == 0) ins_front(k, i);
        else        ins_back(k, i);
    }
    cin >> m;
    while (m--) { int x; cin >> x; del(x); }

    // ★ 教材写法：从【编号 1 的节点】开始往右走（不是从哨兵 a[0].nxt 开始！）
    //   为什么可以：题目保证 p 只指向"已经入队的人"，所以编号 1 永远是最左的人
    //   （后续插入的编号都 ≥ 2，只会插在某个已有人的左/右边，不会插到 1 的左边去）
    int now = indexx[1] ? indexx[1] : a[0].nxt;
    // 若 1 已被删除，则从哨兵的 nxt 开始（等价于"最左的幸存者"）
    //   注：编号 1 被删时 a[0].nxt 已由 del 更新，所以这样兜底是安全的
    while (now) {
        cout << a[now].key << " ";
        now = a[now].nxt;
    }
    cout << "\n";
    return 0;
}
