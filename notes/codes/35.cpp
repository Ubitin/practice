// ============================================================
//  test_/35.cpp —— priority_queue 是否允许元素重复？实测
//
//  配套讲义：讲义/priority_queue是否允许元素重复.md
//  相关：讲义/priority_queue无count-按值计数怎么做.md（那篇讲"怎么数个数"）
//
//  结论先行：**允许**。priority_queue 是【序列容器适配器】（底层 vector），
//            push 只是 push_back + 上浮，没有任何查重步骤。
//            只有【关联容器】（set/map/unordered_*）才有"键唯一"的要求。
// ============================================================
#include <bits/stdc++.h>
using namespace std;

static void title(const char* s) { printf("\n========== %s ==========\n", s); }

struct Node {                       // 自定义结构体（只给 operator<）
    int v;
    string name;
};
bool operator<(const Node& a, const Node& b) { return a.v < b.v; }

int main() {
    // ---------------------------------------------------------
    title("[1] priority_queue：push 三个 5，看是否都保留");
    {
        priority_queue<int> pq;
        pq.push(5); pq.push(5); pq.push(5);
        printf("  push(5) ×3 之后 size() = %zu\n", pq.size());
        printf("  依次 pop：");
        while (!pq.empty()) { printf("%d ", pq.top()); pq.pop(); }
        printf("\n  → 三个 5 全部保留、逐个弹出 ✔ 允许重复\n");
    }

    // ---------------------------------------------------------
    title("[2] 和 set / multiset 对照：能不能存下 3 个 5");
    {
        set<int> s;
        for (int i = 0; i < 3; ++i) {
            auto r = s.insert(5);
            printf("  set::insert(5) 第 %d 次：插入%s，size=%zu\n",
                   i + 1, r.second ? "成功" : "【失败，已存在】", s.size());
        }
        multiset<int> ms;
        for (int i = 0; i < 3; ++i) ms.insert(5);
        printf("  multiset 插入 3 次 5：size = %zu，count(5) = %zu\n", ms.size(), ms.count(5));
        printf("  → set【去重】(size 1)；multiset【保留】(size 3)\n");
    }

    // ---------------------------------------------------------
    title("[3] priority_queue 的 push 有没有返回值？（对比 set::insert）");
    {
        priority_queue<int> pq;
        // pq.push 返回 void —— 连"是否重复"都无从知晓
        printf("  set::insert 返回 pair<iterator,bool>，能告诉你插进去了没有\n");
        printf("  priority_queue::push 返回 void —— 没有任何「插入失败」的概念\n");
        static_assert(is_same<decltype(declval<priority_queue<int>&>().push(1)), void>::value,
                      "push 应返回 void");
        printf("  编译期核实：decltype(pq.push(1)) 就是 void ✔（static_assert 通过）\n");
    }

    // ---------------------------------------------------------
    title("[4] pair / 自定义结构体 也允许完全重复");
    {
        priority_queue<pair<int, int>> pq;
        pq.push({5, 7}); pq.push({5, 7}); pq.push({5, 7});   // 完全相同的 pair
        printf("  push({5,7}) ×3 → size = %zu\n", pq.size());
        while (!pq.empty()) { printf("  (%d,%d) ", pq.top().first, pq.top().second); pq.pop(); }
        printf("\n");

        priority_queue<Node> qn;
        qn.push({5, "a"}); qn.push({5, "b"}); qn.push({5, "c"});  // v 相同
        printf("  push(Node{5,a/b/c}) ×3 → size = %zu（只比 v，三者「等价」，仍全部保留）\n", qn.size());
        while (!qn.empty()) { printf("  {%d,%s} ", qn.top().v, qn.top().name.c_str()); qn.pop(); }
        printf("\n  → 堆只按 < 比较大小，「相等」的元素不会被合并或丢弃\n");
    }

    // ---------------------------------------------------------
    title("[5] 含大量重复值的极端场景");
    {
        const int N = 100000;
        priority_queue<int> pq;
        for (int i = 0; i < N; ++i) pq.push(42);      // 全部相同
        printf("  push(42) × %d → size = %zu，top = %d\n", N, pq.size(), pq.top());
        int cnt = 0;
        while (!pq.empty()) { if (pq.top() == 42) ++cnt; pq.pop(); }
        printf("  全部弹出后数到 %d 个 42\n", cnt);
        printf("  → 重复元素不合并，size() 就是「总个数」（这也是它没有 count 的原因）\n");
    }

    // ---------------------------------------------------------
    title("[6] 汇总：各容器/适配器 允许重复吗");
    {
        printf("  %-34s %-12s %s\n", "容器", "允许重复?", "插入 3 个 5 后的 size");
        {
            priority_queue<int> pq; for (int i=0;i<3;i++) pq.push(5);
            printf("  %-34s %-12s %zu\n", "priority_queue<int>", "✔ 允许", pq.size());
        }
        { vector<int> v; for (int i=0;i<3;i++) v.push_back(5);
          printf("  %-34s %-12s %zu\n", "vector / deque / list", "✔ 允许", v.size()); }
        { multiset<int> m; for (int i=0;i<3;i++) m.insert(5);
          printf("  %-34s %-12s %zu\n", "multiset / multimap", "✔ 允许", m.size()); }
        { set<int> s; for (int i=0;i<3;i++) s.insert(5);
          printf("  %-34s %-12s %zu\n", "set / map", "✘ 键唯一", s.size()); }
        { unordered_set<int> s; for (int i=0;i<3;i++) s.insert(5);
          printf("  %-34s %-12s %zu\n", "unordered_set / unordered_map", "✘ 键唯一", s.size()); }
        { unordered_map<int,int> m; for (int i=0;i<3;i++) m[5]++;
          printf("  %-34s %-12s %zu（但 m[5]=%d）\n", "unordered_map 当计数器用", "✘ 键唯一", m.size(), m[5]); }
        printf("\n  判别标准：它是【序列容器适配器】还是【关联容器】\n");
        printf("    · 序列容器(适配器)：vector/deque/list/stack/queue/priority_queue → 不管值，来就收\n");
        printf("    · 关联容器：set/map/unordered_* → 按键去重；名字带 multi 的才保留重复\n");
    }

    // ---------------------------------------------------------
    title("[7] 常见误解：以为 priority_queue 会去重");
    {
        vector<int> data = {5, 1, 9, 5, 5, 3, 7, 5, 2};
        priority_queue<int> pq;
        for (int x : data) pq.push(x);
        printf("  数据 {5,1,9,5,5,3,7,5,2} 共 %zu 个\n", data.size());
        printf("  priority_queue 的 size() = %zu（没有变成 6）\n", pq.size());
        printf("  → 它保留全部 9 个元素；正因为保留重复，才没法用 count 而要点数表\n");
        printf("     （计数方案见讲义 priority_queue无count-按值计数怎么做）\n");
    }

    printf("\n===== 实测结束 =====\n");
    return 0;
}
