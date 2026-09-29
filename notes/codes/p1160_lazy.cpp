// P1160 惰性删除版（按教材那段"其实删除操作也不是必要的"的提示）
// 删除时不改链表，只打标记；插入也允许参照"被标记删除但仍在链上"的节点
#include <bits/stdc++.h>
using namespace std;

const int maxn = 100005;
struct node { int pre, nxt, key; };
node a[maxn];
int n, m, tot = 0;
int indexx[maxn];
bool dead[maxn];                 // ★ 惰性删除标记

void ins_back(int x, int y) {
    int now = indexx[x], t = ++tot;
    a[t] = {now, a[now].nxt, y};
    a[a[now].nxt].pre = t;
    a[now].nxt = t;
    indexx[y] = t;
}
void ins_front(int x, int y) {
    int now = indexx[x], t = ++tot;
    a[t] = {a[now].pre, now, y};
    a[a[now].pre].nxt = t;
    a[now].pre = t;
    indexx[y] = t;
}

int main() {
    if (!(cin >> n)) return 0;
    tot = 0;
    indexx[1] = ++tot;
    a[tot] = {0, 0, 1};
    for (int i = 2; i <= n; ++i) {
        int k, p; cin >> k >> p;
        if (p == 0) ins_front(k, i); else ins_back(k, i);
    }
    cin >> m;
    while (m--) { int x; cin >> x; dead[x] = true; }    // ★ 只打标记，不动指针

    int now = 1;                                        // 从编号 1 的节点开始
    while (now) {
        if (!dead[a[now].key]) cout << a[now].key << " ";
        now = a[now].nxt;
    }
    cout << "\n";
    return 0;
}
