// 验证 indexx 与 a[].key 的双向映射关系
//   indexx[编号] = 该编号所在的【节点下标】   （编号 → 位置）
//   a[下标].key  = 该节点装的【编号】          （位置 → 编号）
// 两者互为逆映射，但【不是每个下标都有对应编号】（0 号是哨兵；被真删的节点 key 还在但已脱离链）
#include <bits/stdc++.h>
using namespace std;

const int maxn = 100005;
struct node { int pre, nxt, key; };
node a[maxn];
int tot = 0;
int indexx[maxn];
bool dead[maxn];

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
    // 手工造一个小例子，便于逐行核对
    // 操作：2 插到 1 左；3 插到 1 右；4 插到 3 右  ⇒ 期望链：2 1 3 4
    int n = 4;
    a[0] = {0, 0, 0};
    indexx[1] = ++tot; a[tot] = {0, 0, 1};
    a[0].nxt = tot; a[0].pre = tot;
    ins_front(1, 2);      // 2 插到 1 左边
    ins_back(1, 3);       // 3 插到 1 右边
    ins_back(3, 4);       // 4 插到 3 右边

    printf("== 节点池（下标 → 内容）==\n");
    printf("%-6s %-6s %-6s %-8s %s\n", "下标", "pre", "nxt", "key", "indexx[key](回查)");
    for (int i = 0; i <= tot; ++i) {
        printf("%-6d %-6d %-6d %-8d %s\n", i, a[i].pre, a[i].nxt, a[i].key,
               i == 0 ? "(哨兵，无编号)" :
               (indexx[a[i].key] == i ? "与 indexx 一致 ✅" : "不一致 ✘"));
    }

    printf("\n== indexx 索引表（编号 → 节点下标）==\n");
    for (int v = 1; v <= n; ++v) {
        printf("  indexx[%d] = %d   ⇒ 该节点的 key = %d  %s\n",
               v, indexx[v], a[indexx[v]].key,
               (a[indexx[v]].key == v ? "来回一致 ✅" : "不一致 ✘"));
    }

    printf("\n== 顺着 nxt 走出来的队列 ==\n  ");
    for (int now = a[0].nxt; now; now = a[now].nxt) printf("%d ", a[now].key);
    printf("\n  （期望 2 1 3 4）\n");

    // ---- 删除后的不对称演示 ----
    printf("\n== 把 1 号【真删】之后再回查 ==\n");
    int now = indexx[1], le = a[now].pre, rt = a[now].nxt;
    a[le].nxt = rt; a[rt].pre = le;
    indexx[1] = 0;                       // 索引清零
    printf("  indexx[1] = %d        ← 清零了（表示'1 号不在链上'）\n", indexx[1]);
    printf("  a[%d].key = %d        ← 但那个节点的 key 还是 1（真删版没清）\n", now, a[now].key);
    printf("  ⇒ 所以【不能】靠 a[].key 反查位置：索引表才是唯一可信的'编号→位置'来源\n");
    printf("  队列现在是: ");
    for (int p = a[0].nxt; p; p = a[p].nxt) printf("%d ", a[p].key);
    printf("\n  （期望 2 3 4）\n");
    return 0;
}
