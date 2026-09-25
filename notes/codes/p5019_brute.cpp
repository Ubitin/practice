// P5019 铺设道路 —— 独立暴力验证器
//
// 题意：n 段路，第 i 段需要填 a[i] 单位土。每次操作选一段【连续区间】 [l,r]，
//       把区间内每段各填 1 单位（不能超填）。求填满所需最少操作次数。
//
// 暴力思路（与 Σmax(0, a[i]-a[i-1]) 完全不同的另一套推理）：
//   BFS / 记忆化搜索 直接枚举"下一步选哪个区间"，直到全 0，取最少步数。
//   虽然慢，但**逻辑上无任何捷径**，可作为标准答案来对拍公式解。
//
// 输出：不同规模下的暴力值 vs 公式值，以及不一致的用例（若有）
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

map<vector<int>, int> memo;
long long nodes = 0;

// 返回把 s 全部归零的最少操作次数
int solve(vector<int>& s, int n) {
    bool allz = true;
    for (int i = 0; i < n; ++i) if (s[i]) { allz = false; break; }
    if (allz) return 0;
    auto it = memo.find(s);
    if (it != memo.end()) return it->second;
    ++nodes;

    int best = INT_MAX;
    for (int l = 0; l < n; ++l) {
        if (s[l] == 0) continue;                 // 左端必须还有坑要填
        for (int r = l; r < n; ++r) {
            if (s[r] == 0) break;                // 区间中不能包含已填满的段
            bool ok = true;
            for (int k = l; k <= r; ++k) if (s[k] == 0) { ok = false; break; }
            if (!ok) break;
            for (int k = l; k <= r; ++k) --s[k]; // 填一层
            best = min(best, 1 + solve(s, n));
            for (int k = l; k <= r; ++k) ++s[k]; // 回溯
        }
    }
    memo[s] = best;
    return best;
}

long long formula(const vector<int>& a) {
    long long ans = a[0];
    for (size_t i = 1; i < a.size(); ++i)
        if (a[i] > a[i - 1]) ans += a[i] - a[i - 1];
    return ans;
}

int main() {
    // ---------- 一、穷举所有小用例（h<=5, n<=6） ----------
    long long total = 0, mismatch = 0;
    long long firstBadN = -1; vector<int> firstBad;
    for (int n = 1; n <= 6; ++n) {
        vector<int> a(n, 0);
        while (true) {
            memo.clear();
            vector<int> s = a;
            int brute = solve(s, n);
            long long f = formula(a);
            ++total;
            if (brute != f) {
                ++mismatch;
                if (firstBadN < 0) { firstBadN = n; firstBad = a; }
            }
            // 下一个用例（h 从 1..5，注意用 1..5 避免全 0 占位）
            int p = n - 1;
            while (p >= 0) { if (++a[p] <= 5) break; a[p] = 1; --p; }
            if (p < 0) break;
        }
    }
    printf("穷举 h∈[1,5], n∈[1,6]：共 %lld 个用例，不一致 %lld 个\n", total, mismatch);
    if (mismatch) {
        printf("  首个反例 n=%lld 数组 =", firstBadN);
        for (int x : firstBad) printf(" %d", x);
        printf("\n");
    }

    // ---------- 二、随机中用例（h<=8, n<=8），暴力仍可承受 ----------
    mt19937 rng(12345);
    long long t2 = 0, bad2 = 0;
    for (int t = 0; t < 300; ++t) {
        int n = 1 + rng() % 8;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) a[i] = 1 + rng() % 8;
        memo.clear();
        vector<int> s = a;
        int brute = solve(s, n);
        ++t2;
        if (brute != formula(a)) {
            ++bad2;
            if (bad2 <= 3) {
                printf("  反例:"); for (int x : a) printf(" %d", x); printf(" 暴力=%d 公式=%lld\n", brute, formula(a));
            }
        }
    }
    printf("随机 h∈[1,8], n∈[1,8]：共 %lld 个用例，不一致 %lld 个\n", t2, bad2);
    return 0;
}
