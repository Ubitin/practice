// 「DP 的题能不能用贪心」—— 用三个经典模型做实测对照
//   ① 0/1 背包：贪心按性价比拿 → 错多少？
//   ② 数字三角形：贪心每步选大的孩子 → 错多少？
//   ③ 找零钱：贪心每次用最大的硬币 → 什么情况下错？
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ---------------- ① 0/1 背包 ----------------
ll knapDP(const vector<ll>& w, const vector<ll>& v, ll C) {
    vector<ll> dp(C + 1, 0);
    for (size_t i = 0; i < w.size(); ++i)
        for (ll c = C; c >= w[i]; --c)          // 倒序 = 每件只拿一次
            dp[c] = max(dp[c], dp[c - w[i]] + v[i]);
    return dp[C];
}
// 贪心：按性价比 v/w 从大到小，能装就装（不许后悔）
ll knapGreedy(vector<ll> w, vector<ll> v, ll C) {
    int n = w.size();
    vector<int> id(n); iota(id.begin(), id.end(), 0);
    sort(id.begin(), id.end(), [&](int a, int b) {
        return (long double)v[a] / w[a] > (long double)v[b] / w[b];
    });
    ll tot = 0;
    for (int k = 0; k < n; ++k) { int i = id[k]; if (w[i] <= C) { C -= w[i]; tot += v[i]; } }
    return tot;
}

// ---------------- ② 数字三角形 ----------------
ll triDP(const vector<vector<ll>>& a) {
    int n = a.size();
    vector<ll> dp = a[n - 1];
    for (int i = n - 2; i >= 0; --i)
        for (int j = 0; j <= i; ++j)
            dp[j] = a[i][j] + max(dp[j], dp[j + 1]);
    return dp[0];
}
ll triGreedy(const vector<vector<ll>>& a) {           // 每步只看下面两个较大的那个
    int n = a.size(), j = 0; ll s = a[0][0];
    for (int i = 0; i + 1 < n; ++i) { j += (a[i + 1][j] >= a[i + 1][j + 1]) ? 0 : 1; s += a[i + 1][j]; }
    return s;
}

// ---------------- ③ 找零钱 ----------------
int coinDP(const vector<int>& c, int M) {             // 最少硬币数（无限个）
    const int INF = 1e9;
    vector<int> dp(M + 1, INF); dp[0] = 0;
    for (int m = 1; m <= M; ++m) for (int x : c) if (x <= m) dp[m] = min(dp[m], dp[m - x] + 1);
    return dp[M] >= INF ? -1 : dp[M];
}
int coinGreedy(vector<int> c, int M) {                // 每次用不超过余额的最大面额
    sort(c.rbegin(), c.rend());
    int cnt = 0;
    for (int x : c) { cnt += M / x; M %= x; }
    return M ? -1 : cnt;
}

int main() {
    mt19937 rng(20260926);

    // ① 0/1 背包：小规模穷举 + 随机扫描，统计贪心错的比例与最大偏小幅度
    {
        long long total = 0, bad = 0, worst = 0; vector<ll> wi, vi; ll Cw = 0;
        for (int n = 2; n <= 6; ++n) {
            vector<ll> w(n), v(n);
            while (true) {
                ll C = 1 + rng() % 20;
                for (int i = 0; i < n; ++i) { w[i] = 1 + rng() % 12; v[i] = 1 + rng() % 12; }
                ll d = knapDP(w, v, C), g = knapGreedy(w, v, C);
                ++total;
                if (g != d) { ++bad; if (d - g > worst) { worst = d - g; wi = w; vi = v; Cw = C; } }
                if (total >= 20000) break;
            }
            if (total >= 20000) break;
        }
        printf("① 0/1 背包：随机 %lld 组，贪心错 %lld 组（%.1f%%），最坏比最优少 %lld\n",
               total, bad, 100.0 * bad / total, worst);
        printf("   经典反例：容量 %lld，物品 (w,v) =", Cw);
        for (size_t i = 0; i < wi.size(); ++i) printf(" (%lld,%lld)", wi[i], vi[i]);
        printf("\n   贪心=%lld  最优=%lld\n", knapGreedy(wi, vi, Cw), knapDP(wi, vi, Cw));
    }

    // ② 数字三角形：随机生成，统计贪心错的比例
    {
        long long total = 0, bad = 0, worst = 0;
        vector<vector<ll>> worstTri; 
        for (int t = 0; t < 5000; ++t) {
            int n = 2 + rng() % 8;
            vector<vector<ll>> a(n);
            for (int i = 0; i < n; ++i) { a[i].resize(i + 1); for (int j = 0; j <= i; ++j) a[i][j] = rng() % 20; }
            ll d = triDP(a), g = triGreedy(a);
            ++total;
            if (g != d) { ++bad; if (d - g > worst) { worst = d - g; worstTri = a; } }
        }
        printf("② 数字三角形：随机 %lld 组，贪心错 %lld 组（%.1f%%），最坏比最优少 %lld\n",
               total, bad, 100.0 * bad / total, worst);
        if (!worstTri.empty()) {
            printf("   最坏反例：\n");
            for (auto& r : worstTri) { printf("     "); for (ll x : r) printf("%2lld ", x); printf("\n"); }
            printf("   贪心=%lld  最优=%lld\n", triGreedy(worstTri), triDP(worstTri));
        }
    }

    // ③ 找零钱：固定面额集合，找出贪心出错的最小金额
    {
        vector<vector<int>> sets = {{1,3,4},{1,5,6,9},{1,4,5,10},{1,3,7,10}};
        for (auto& c : sets) {
            int firstBad = -1; int g = 0, d = 0;
            for (int M = 1; M <= 200; ++M) {
                int gg = coinGreedy(c, M), dd = coinDP(c, M);
                if (gg != dd) { firstBad = M; g = gg; d = dd; break; }
            }
            printf("③ 面额 {");
            for (size_t i = 0; i < c.size(); ++i) printf("%d%s", c[i], i + 1 < c.size() ? "," : "");
            if (firstBad < 0) printf("}：1..200 内贪心全对（这类面额叫「规范面额」）\n");
            else printf("}：最小反例 M=%d 贪心要给 %d 枚，最优只要 %d 枚\n", firstBad, g, d);
        }
    }
    return 0;
}
