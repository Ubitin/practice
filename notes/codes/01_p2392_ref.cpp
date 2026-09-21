// ============================================================
//  01_p2392_ref.cpp —— P2392 考前临时抱佛脚（独立参考解：0/1 背包）
//
//  每个科目独立处理：把该科题目分成左右两份，使 max(左, 右) 最小
//    ⟺ 让较小那份尽可能大，但不超过 sum/2
//    ⟺ 【容量为 sum/2 的 0/1 背包，求最大可达和 best】
//    该科用时 = sum - best
//  四个科目串行，答案 = Σ(sum_i - best_i)
//
//  这份用【布尔 DP】实现，与 test_/01.cpp 的【DFS 枚举子集】是完全不同的算法，
//  正好可以互相印证。
// ============================================================
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int s[4];
    for (int i = 0; i < 4; ++i) cin >> s[i];

    long long ans = 0;
    for (int i = 0; i < 4; ++i) {
        vector<int> a(s[i]);
        int sum = 0;
        for (int j = 0; j < s[i]; ++j) { cin >> a[j]; sum += a[j]; }

        int cap = sum / 2;                       // 容量 = ⌊sum/2⌋
        vector<char> dp(cap + 1, 0);
        dp[0] = 1;
        for (int x : a)
            for (int j = cap; j >= x; --j)       // ★ 0/1 背包必须倒序
                if (dp[j - x]) dp[j] = 1;

        int best = 0;
        for (int j = cap; j >= 0; --j) if (dp[j]) { best = j; break; }

        ans += (long long)sum - best;
    }
    cout << ans << '\n';
    return 0;
}
