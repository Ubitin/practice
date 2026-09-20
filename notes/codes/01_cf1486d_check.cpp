// ============================================================
//  01_cf1486d_check.cpp —— CF1486D：定位原码错误 + 验证正确解法
//
//  原文件 test_/01.cpp 保持原样、未做任何改动。
//
//  原码逻辑：把所有数塞进大根堆，弹出 t = ⌊k/2⌋+1 次，输出最后弹出的那个。
//            这是【全局第 t 大】，与"连续子段的中位数"无关。
// ============================================================
#include <bits/stdc++.h>
using namespace std;

// ---------- 原码 ----------
static int solve_user(const vector<int>& a, int k) {
    priority_queue<int> dp;
    for (int x : a) dp.push(x);
    int t = (k + 1) / 2;
    t = k - t + 1;
    int ans = 0;
    while (t--) { ans = dp.top(); dp.pop(); }
    return ans;
}

// ---------- 正确解法：二分答案 + 前缀最小值 ----------
// strictSum = true  → 子段和 > 0  → 下中位数（第 ⌈m/2⌉ 小）
// strictSum = false → 子段和 ≥ 0  → 上中位数（第 ⌈(m+1)/2⌉ 小）
static int solve_fixed(const vector<int>& a, int k, bool strictSum = true) {
    int n = a.size();
    int mx = *max_element(a.begin(), a.end());
    vector<int> pre(n + 1);
    auto check = [&](int x) {
        pre[0] = 0;
        for (int i = 0; i < n; ++i) pre[i + 1] = pre[i] + (a[i] >= x ? 1 : -1);
        int mn = INT_MAX;
        for (int j = k; j <= n; ++j) {
            mn = min(mn, pre[j - k]);
            if (strictSum ? (pre[j] > mn) : (pre[j] >= mn)) return true;
        }
        return false;
    };
    int lo = 1, hi = mx;
    while (lo < hi) {
        int mid = lo + (hi - lo + 1) / 2;
        if (check(mid)) lo = mid; else hi = mid - 1;
    }
    return lo;
}

// ---------- 暴力：枚举所有长度 ≥ k 的子段 ----------
// upper = true 取上中位数 ⌈(m+1)/2⌉ 小，false 取下中位数 ⌈m/2⌉ 小
static int brute(const vector<int>& a, int k, bool upper = false) {
    int n = a.size(), best = INT_MIN;
    for (int l = 0; l < n; ++l) {
        vector<int> seg;
        for (int r = l; r < n; ++r) {
            seg.push_back(a[r]);
            int m = r - l + 1;
            if (m < k) continue;
            vector<int> s = seg;
            sort(s.begin(), s.end());
            int idx = (upper ? (m + 2) / 2 : (m + 1) / 2) - 1;   // 转 0-based
            best = max(best, s[idx]);
        }
    }
    return best;
}

int main() {
    cout << "===== [1] 关键反例：n=5, k=3, a={5,1,1,1,5} =====\n";
    {
        vector<int> a = {5, 1, 1, 1, 5};
        printf("  原码输出          = %d\n", solve_user(a, 3));
        printf("  正确答案(下中位数) = %d\n", brute(a, 3, false));
        printf("  正确答案(上中位数) = %d\n", brute(a, 3, true));
        printf("  枚举长度为 3~5 的所有子段的中位数：\n");
        for (int l = 0; l < 5; ++l)
            for (int r = l; r < 5; ++r) {
                if (r - l + 1 < 3) continue;
                vector<int> s(a.begin() + l, a.begin() + r + 1);
                sort(s.begin(), s.end());
                printf("    [%d..%d] = {", l + 1, r + 1);
                for (size_t i = 0; i < s.size(); ++i) printf("%d%s", s[i], i + 1 < s.size() ? "," : "");
                printf("}  m=%d  下中位数=第%d小=%d\n", (int)s.size(), (int)s.size() / 2 + 1, s[(s.size() + 1) / 2 - 1]);
            }
        printf("  → 5 从来没出现在任何子段的中间位置；原码却输出了 5\n");
        printf("  → 两种中位数定义下正确答案都是 1，原码都是错的\n");
    }

    cout << "\n===== [2] 官方样例：原码为什么「能过」 =====\n";
    {
        vector<int> a = {1, 2, 3, 4, 5};
        printf("  n=5, k=3, a={1,2,3,4,5}\n");
        printf("  原码输出          = %d\n", solve_user(a, 3));
        printf("  正确答案(下中位数) = %d\n", brute(a, 3, false));
        printf("  正确答案(上中位数) = %d\n", brute(a, 3, true));
        printf("  → 凑巧相等（样例答案 4），所以看不出问题\n");
    }

    cout << "\n===== [3] 原码什么时候是对的？ =====\n";
    {
        printf("  当 n == k 时，唯一合法子段就是整个数组，其下中位数 = 第 ⌈k/2⌉ 小\n");
        printf("  而原码输出的是第 (k-⌊k/2⌋) 小 = 第 ⌈k/2⌉ 小 —— 恰好相等\n");
        mt19937_64 rng(1);
        int ok = 0, tot = 0;
        for (int tc = 0; tc < 2000; ++tc) {
            int n = 1 + rng() % 8;
            vector<int> a(n);
            for (auto& x : a) x = (int)(1 + rng() % n);
            int k = n;                                  // n == k
            ++tot;
            if (solve_user(a, k) == brute(a, k, false)) ++ok;
        }
        printf("  实测 n==k 的 %d 组：原码与正确解一致 %d 组\n", tot, ok);
        printf("  → 也就是说：只有「没有选择余地」时它才对\n");
    }

    cout << "\n===== [4] 随机对拍：原码 vs 正确解（n > k）=====\n";
    {
        mt19937_64 rng(20260917);
        int badUser = 0, badFixed = 0, badFixedUpper = 0, tot = 0;
        int bigger = 0;
        for (int tc = 0; tc < 20000; ++tc) {
            int n = 2 + rng() % 9;
            int k = 1 + rng() % (n - 1);                // 保证 k < n
            vector<int> a(n);
            for (auto& x : a) x = (int)(1 + rng() % n);
            int eL = brute(a, k, false), eU = brute(a, k, true);
            ++tot;
            if (solve_user(a, k) != eL) { ++badUser; if (solve_user(a, k) > eL) ++bigger; }
            if (solve_fixed(a, k, true)  != eL) ++badFixed;
            if (solve_fixed(a, k, false) != eU) ++badFixedUpper;
        }
        printf("  20000 组：\n");
        printf("    原码          与下中位数答案不符 %d 组（其中偏大 %d 组）\n", badUser, bigger);
        printf("    正确解(下中位数) 不符 %d 组\n", badFixed);
        printf("    正确解(上中位数) 不符 %d 组\n", badFixedUpper);
        printf("  → 正确解在两种定义下都与暴力完全一致；原码错误率极高\n");
    }

    cout << "\n===== [5] 正确解回归 =====\n";
    {
        struct { vector<int> a; int k; } T[] = {
            {{1,2,3,4,5}, 3},      // 官方样例 → 4
            {{5,1,1,1,5}, 3},      // 反例 → 1
            {{1,2,3,4},   2},      // → 3（下中位数）
            {{1},         1},      // → 1
            {{2,1,2,1,2}, 3},      // → 2
        };
        for (auto& t : T) {
            int f = solve_fixed(t.a, t.k, true);
            int b = brute(t.a, t.k, false);
            printf("  %s n=%zu k=%d → 正确解 %d，暴力 %d\n",
                   f == b ? "OK " : "!! ", t.a.size(), t.k, f, b);
        }
    }
    return 0;
}
