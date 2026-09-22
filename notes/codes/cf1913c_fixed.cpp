// CF1913C Game with Multiset —— 修正版（保留你原来的骨架，只换掉核心判据）
// 你原来的写法：p 累加所有元素 → 用 (w & p) == w 判。
// 问题：p 是【和】，一个和丢掉了"有几个"的信息（两个 1 加起来就变成 2 了）。
// 正解：按指数存个数 cnt[i]，查询时从低位往高位"进位贪心"。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll m = 0;
ll cnt[40] = {0};   // cnt[i] = 多重集里 2^i 的个数（x ≤ 29，开 40 够）

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> m;

    for (ll i = 0; i < m; ++i) {
        ll t, v;
        cin >> t >> v;
        if (t == 1) {
            cnt[v]++;                 // ★ v 是【指数】：加入 2^v，不能只加 v，也不能直接扔进一个和
        } else {
            ll have = 0;              // 当前可用的 2^i 个数（含低位进位来的）
            bool ok = true;
            for (int b = 0; b <= 30; ++b) {   // w ≤ 1e9 < 2^30
                have += cnt[b];
                if ((v >> b) & 1) {           // w 的第 b 位是 1，必须消耗一个 2^b
                    if (have == 0) { ok = false; break; }
                    --have;
                }
                have >>= 1;                   // 剩下的两个 2^b 合成一个 2^(b+1)
            }
            cout << (ok ? "YES\n" : "NO\n");
        }
    }
    return 0;
}
