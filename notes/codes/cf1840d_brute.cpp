// CF1840D —— 独立暴力（枚举"切两刀"，与"二分答案+贪心覆盖"是两套完全不同的推理）
// 原理：把点排序后，最优方案一定可以看成把序列切成 ≤3 段连续区间（每段交给一个工匠）。
//       一段 [p,q] 用一个【整数型号】覆盖的最小代价 = ceil((q-p)/2) = (q-p+1)/2。
//       枚举两刀的切点，取"三段代价的最大值"的最小值。
// 复杂度 O(n^2)，与值域无关（所以能处理 a_i 到 1e9 的数据）。
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n;
        cin >> n;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        sort(a.begin(), a.end());
        long long best = LLONG_MAX;
        for (int i = 0; i <= n; ++i)          // 第一段 [0, i)
            for (int j = i; j <= n; ++j) {    // 第二段 [i, j)，第三段 [j, n)
                long long w = 0;
                if (i > 0)   w = max(w, (a[i - 1] - a[0] + 1) / 2);
                if (j > i)   w = max(w, (a[j - 1] - a[i] + 1) / 2);
                if (n > j)   w = max(w, (a[n - 1] - a[j] + 1) / 2);
                best = min(best, w);
            }
        cout << best << "\n";
    }
    return 0;
}
