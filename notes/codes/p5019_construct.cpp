// P5019：把"最少次数"变成"能真的铺出来" —— 构造一组具体操作并逐格核对
//
// 构造规则（朴素的"向右贪心"）：
//   从左往右扫每一段 i；若 a[i] > a[i-1]，说明第 i 段比左边高出一截，
//   这一截必须由【新的操作】来填，于是新开 (a[i]-a[i-1]) 次操作，
//   每次操作从 i 开始尽量向右延伸（延伸到右边不再需要的段为止）。
//   同时输出这些操作，最后逐格检验：每段被填的总数 == a[i]，且没有任何一次超填。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    vector<ll> a;
    int n;
    if (!(cin >> n)) return 0;
    a.assign(n, 0);
    for (int i = 0; i < n; ++i) cin >> a[i];

    vector<ll> fill(n, 0);                 // 模拟：每段累计被填了多少
    vector<pair<int,int>> ops;             // 记录每次操作的区间（1-based）
    ll prev = 0;
    for (int i = 0; i < n; ++i) {
        if (a[i] > prev) {
            ll need = a[i] - prev;         // 必须新开的操作次数
            for (ll t = 0; t < need; ++t) {
                // 从 i 开始尽量往右延伸：只要右边那段的"剩余需求"还够
                int r = i;
                while (r + 1 < n && a[r + 1] > fill[r + 1]) ++r;
                for (int k = i; k <= r; ++k) ++fill[k];
                ops.push_back({i + 1, r + 1});
            }
        }
        prev = a[i];
    }

    // ---- 校验 1：每段恰好被填 a[i] 次 ----
    bool ok1 = true;
    for (int i = 0; i < n; ++i) if (fill[i] != a[i]) ok1 = false;

    // ---- 校验 2：每次操作都是合法连续区间，且区间内每段都还没填满（不超填）----
    bool ok2 = true;
    vector<ll> sim(n, 0);
    for (auto& op : ops) {
        if (op.first < 1 || op.second > n || op.first > op.second) { ok2 = false; break; }
        for (int k = op.first - 1; k <= op.second - 1; ++k) {
            ++sim[k];
            if (sim[k] > a[k]) { ok2 = false; break; }   // 超填 = 非法
        }
        if (!ok2) break;
    }
    bool ok3 = true;
    for (int i = 0; i < n; ++i) if (sim[i] != a[i]) ok3 = false;

    // ---- 计算公式给出的下界 ----
    ll formula = a[0];
    for (int i = 1; i < n; ++i) if (a[i] > a[i - 1]) formula += a[i] - a[i - 1];

    printf("数组:");
    for (int i = 0; i < n; ++i) printf(" %lld", a[i]);
    printf("\n操作次数（本构造）= %d，公式下界 = %lld，是否相等：%s\n",
           (int)ops.size(), formula, (ops.size() == (size_t)formula) ? "是 ✅" : "否 ✘");
    printf("每段被填总数 == a[i]：%s   区间合法且不超填：%s   最终恰好填满：%s\n",
           ok1 ? "✅" : "✘", ok2 ? "✅" : "✘", ok3 ? "✅" : "✘");
    printf("具体操作序列：\n");
    for (size_t k = 0; k < ops.size(); ++k)
        printf("  第 %2d 次：对路段 [%d, %d] 各填 1 单位\n", (int)k + 1, ops[k].first, ops[k].second);
    return (ok1 && ok2 && ok3) ? 0 : 7;
}
