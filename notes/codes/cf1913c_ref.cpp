// CF1913C Game with Multiset —— 正确解（进位贪心）
// 1 x : 往多重集里加入 2^x（x 是【指数】，0 ≤ x ≤ 29）
// 2 w : 问能否选出若干元素使其和恰为 w（1 ≤ w ≤ 1e9）
//
// 做法：cnt[i] 记 2^i 的个数。对 w 从低位往高位扫：
//   have = 当前可用的 2^i 个数（含从低位进位来的）
//   若 w 的第 i 位是 1，就必须消耗一个 2^i；没有则失败
//   剩下的每两个 2^i 可以合成一个 2^(i+1) → have >>= 1
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    if (!(cin >> q)) return 0;
    long long cnt[40] = {0};
    while (q--) {
        int t;
        long long v;
        cin >> t >> v;
        if (t == 1) {
            cnt[v]++;                       // v 是指数
        } else {
            long long have = 0;
            bool ok = true;
            for (int i = 0; i <= 30; ++i) { // w ≤ 1e9 < 2^30
                have += cnt[i];
                if ((v >> i) & 1) {
                    if (have == 0) { ok = false; break; }
                    --have;
                }
                have >>= 1;
            }
            cout << (ok ? "YES" : "NO") << "\n";
        }
    }
    return 0;
}
