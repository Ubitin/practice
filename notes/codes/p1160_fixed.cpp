// P1160 修正版：补回 del 里漏掉的 a[le].nxt = rt;
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

void ins_back(int x, int y) {          // 把 y 插到 x 后面
    int now = indexx[x];
    int t = ++tot;
    a[t] = node(now, a[now].nxt, y);
    a[a[now].nxt].pre = t;
    a[now].nxt = t;
    indexx[y] = t;
}
void ins_front(int x, int y) {         // 把 y 插到 x 前面
    int now = indexx[x];
    int t = ++tot;
    a[t] = node(a[now].pre, now, y);
    a[a[now].pre].nxt = t;             // ★ 与 ins_back 对称的两行都要写
    a[now].pre = t;
    indexx[y] = t;
}
void del(int x) {
    if (!indexx[x]) return;
    int now = indexx[x];
    int le = a[now].pre, rt = a[now].nxt;
    a[le].nxt = rt;                    // ★★ 这行是关键的半句：改左邻居的 nxt
    a[rt].pre = le;                    //    改右邻居的 pre（教材有，我上一版漏了上一行）
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
    for (int now = a[0].nxt; now; now = a[now].nxt)   // 哨兵的 nxt 就是最左
        cout << a[now].key << " ";
    cout << "\n";
    return 0;
}
