// 补充实验：① 更干净的 0/1 背包最小反例；② P1216 小规模数字三角形上贪心的错误率
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll knapDP(const vector<ll>& w, const vector<ll>& v, ll C) {
    vector<ll> dp(C + 1, 0);
    for (size_t i = 0; i < w.size(); ++i)
        for (ll c = C; c >= w[i]; --c) dp[c] = max(dp[c], dp[c - w[i]] + v[i]);
    return dp[C];
}
ll knapGreedy(vector<ll> w, vector<ll> v, ll C) {
    int n = w.size(); vector<int> id(n); iota(id.begin(), id.end(), 0);
    sort(id.begin(), id.end(), [&](int a, int b) { return (long double)v[a] / w[a] > (long double)v[b] / w[b]; });
    ll t = 0; for (int k = 0; k < n; ++k) { int i = id[k]; if (w[i] <= C) { C -= w[i]; t += v[i]; } }
    return t;
}
int main() {
    // ① 刻意构造：性价比最高的一件把容量占满，但两件小的加起来更值
    struct T { ll C; vector<ll> w, v; };
    vector<T> ts = {
        {10, {6, 5, 5}, {6, 5, 5}},          // 性价比 (6/6=1) 最高 → 拿它剩 4 装不下 → 6；最优 5+5=10
        {5,  {5, 3, 3}, {5, 4, 4}},          // 贪心拿 5 → 5；最优 3+3=8
        {10, {7, 5, 5}, {7, 6, 6}},          // 贪心拿 7 → 7；最优 5+5=12
    };
    printf("① 构造的反例（性价比贪心 vs DP）\n");
    for (auto& t : ts) {
        printf("   容量 %2lld 物品", t.C);
        for (size_t i = 0; i < t.w.size(); ++i) printf(" (w=%lld,v=%lld)", t.w[i], t.v[i]);
        printf("  →  贪心=%lld  最优=%lld  %s\n", knapGreedy(t.w, t.v, t.C), knapDP(t.w, t.v, t.C),
               knapGreedy(t.w, t.v, t.C) == knapDP(t.w, t.v, t.C) ? "" : "✘ 贪心错");
    }

    // ② 数字三角形：值域改成 0/1（最接近"每步选大的"直觉），看 2/3/4 层小规模
    printf("\n② 数字三角形（行数 2..6，值域 0..9）贪心错误率\n");
    mt19937 rng(7);
    for (int n = 2; n <= 6; ++n) {
        int bad = 0, total = 0;
        for (int t = 0; t < 2000; ++t) {
            vector<vector<ll>> a(n);
            for (int i = 0; i < n; ++i) { a[i].resize(i + 1); for (int j = 0; j <= i; ++j) a[i][j] = rng() % 10; }
            vector<ll> dp = a[n - 1];
            for (int i = n - 2; i >= 0; --i) for (int j = 0; j <= i; ++j) dp[j] = a[i][j] + max(dp[j], dp[j + 1]);
            int j = 0; ll s = a[0][0];
            for (int i = 0; i + 1 < n; ++i) { j += (a[i + 1][j] >= a[i + 1][j + 1]) ? 0 : 1; s += a[i + 1][j]; }
            ++total; if (s != dp[0]) ++bad;
        }
        printf("   行数 %d：贪心错 %d / %d = %.1f%%\n", n, bad, total, 100.0 * bad / total);
    }
    return 0;
}
