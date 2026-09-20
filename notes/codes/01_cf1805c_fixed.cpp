// CF 1805C Place for a Selfie —— 正解：排序 + 二分（取离 b 最近的斜率 k）
// 原代码是"每条抛物线都线性扫描所有直线"→ O(n·m) → TLE
#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while (t--) {
        int n, m; cin >> n >> m;
        vector<long long> k(n);
        for (int i = 0; i < n; ++i) cin >> k[i];
        sort(k.begin(), k.end());                       // 显式排序，不依赖输入有序

        for (int i = 0; i < m; ++i) {
            long long a, b, c; cin >> a >> b >> c;
            // 需要 (b - k)^2 < 4ac，即 |b - k| < 2*sqrt(ac)
            // => k 离 b 越近越容易满足，只需检查"离 b 最近的 k"
            auto it = lower_bound(k.begin(), k.end(), b);   // 第一个 >= b
            bool ok = false;
            long long ans = 0;
            for (auto cand : {it, it == k.begin() ? k.end() : prev(it)}) {  // 只看 b 的左右邻居
                if (cand == k.end()) continue;
                long long d = b - *cand;
                if (d * d < 4 * a * c) { ok = true; ans = *cand; break; }
            }
            if (ok) cout << "YES\n" << ans << "\n";
            else    cout << "NO\n";
        }
    }
    return 0;
}
