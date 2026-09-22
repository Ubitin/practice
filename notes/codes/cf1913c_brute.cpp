// CF1913C —— 独立暴力解：把多重集原样存下来，每次查询做一次子集和 bitset DP
// 与"进位贪心"是完全不同的思路，用来交叉验证。
// 注意：CAP 必须是 2 的幂且 ≥ 生成器里最大的 w。
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int CAP = 1 << 18;   // 262144，生成器会把 w 和总和都限制在这以下
    int q;
    if (!(cin >> q)) return 0;
    vector<int> vals;
    while (q--) {
        int t;
        long long v;
        cin >> t >> v;
        if (t == 1) {
            vals.push_back(1 << (int)v);
        } else {
            bitset<CAP + 1> dp;
            dp.reset();
            dp[0] = 1;
            for (int x : vals) {
                if (x <= CAP) dp |= (dp << x);
            }
            bool ok = (v <= CAP) && dp.test((size_t)v);
            cout << (ok ? "YES" : "NO") << "\n";
        }
    }
    return 0;
}
