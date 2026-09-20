// ============================================================
//  01_bsanswer_verify.cpp —— 验证"二分下标"为什么是错的
//
//  原文件 test_/01.cpp 保持原样、未做任何改动。
//
//  原码二分的是【下标】(v[mid])，等价于假定"答案一定是某个树高"。
//  本程序用暴力枚举做基准，量化这个假定的错误率。
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

static ll wood(const vector<ll>& a, ll H) {      // 锯片高度 H 得到的木材总量
    ll s = 0;
    for (ll x : a) if (x > H) s += x - H;
    return s;
}

// 基准：从 max 往小枚举，第一个 W(H) >= M 的 H 就是答案（数据小才敢这么做）
static ll brute(const vector<ll>& a, ll M) {
    ll mx = *max_element(a.begin(), a.end());
    for (ll H = mx; H >= 0; --H)
        if (wood(a, H) >= M) return H;
    return 0;
}

// 正解：二分【值域】[0, max]
static ll bs_value(vector<ll> a, ll M) {
    ll lo = 0, hi = *max_element(a.begin(), a.end());
    while (lo < hi) {
        ll mid = lo + (hi - lo + 1) / 2;
        if (wood(a, mid) >= M) lo = mid;
        else                   hi = mid - 1;
    }
    return lo;
}

// 原码思路：二分【下标】（已修好死循环，只保留"答案必是某个树高"这个错误假定）
static ll bs_index(vector<ll> a, ll M) {
    sort(a.begin(), a.end());
    ll n = a.size(), l = 0, r = n - 1, ans = 0;
    while (l <= r) {
        ll mid = l + (r - l) / 2;
        if (wood(a, a[mid]) >= M) { ans = a[mid]; l = mid + 1; }
        else                      { r = mid - 1; }
    }
    return ans;
}

int main() {
    cout << "===== [1] 反例：答案根本不在数组里 =====\n";
    {
        vector<ll> a = {20, 15};
        ll M = 7;
        cout << "a = {20, 15}, M = 7\n";
        for (ll H = 17; H >= 12; --H)
            printf("  H=%2lld → W(H)=%2lld  %s\n", H, wood(a, H), wood(a, H) >= M ? ">= M ✔" : "< M  ✘");
        cout << "  → 最大可行 H = " << brute(a, M) << "，而 14 不在 {20,15} 中\n";
        cout << "  值域二分得 " << bs_value(a, M) << "（✔）   下标域二分得 " << bs_index(a, M) << "（✘）\n";
    }

    cout << "\n===== [2] 随机数据统计（每棵树高 <= 40，M 随机）=====\n";
    {
        mt19937_64 rng(20260917);
        int badValue = 0, badIndex = 0, notInArray = 0, total = 0;
        for (int tc = 0; tc < 30000; ++tc) {
            int n = 2 + rng() % 6;
            vector<ll> a(n);
            for (auto& x : a) x = 1 + rng() % 40;
            ll mx = *max_element(a.begin(), a.end());
            ll M = 1 + rng() % (mx * n + 1);          // 保证有解，答案不会落到 0 的边界
            ll expect = brute(a, M);
            total++;
            if (bs_value(a, M) != expect) badValue++;
            if (bs_index(a, M) != expect) badIndex++;
            bool inArr = (find(a.begin(), a.end(), expect) != a.end());
            if (!inArr) notInArray++;
        }
        cout << "30000 组随机数据：\n";
        cout << "  值域二分  错 " << badValue  << " 组\n";
        cout << "  下标域二分 错 " << badIndex << " 组\n";
        cout << "  真答案【不在】树高数组里的情况：" << notInArray << " 组（占 "
             << fixed << setprecision(1) << (100.0 * notInArray / total) << "%）\n";
        cout << "  → 下标域二分一旦碰上这种数据就必然 WA\n";
    }

    cout << "\n===== [3] P1873 砍树 官方样例回归 =====\n";
    {
        vector<ll> a = {20, 15, 10, 17};
        ll M = 7;
        cout << "输入 4 7 / 20 15 10 17 → 期望 15，实得 "
             << bs_value(a, M) << "\n";
        cout << "  （这组样例的答案 15 恰好在数组里，所以下标域二分也蒙对了："
             << bs_index(a, M) << "）\n";
        cout << "  ⚠️ 这正是最危险的地方：样例过了，交上去照样 WA\n";
    }
    return 0;
}
