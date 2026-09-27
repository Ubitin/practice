// NWPUPC 2026 Problem D —— 正解：二分答案（对 R 二分），O(N log x)
// 题意：N 个点坐标 x[1..N]（严格递增），通信距离 R 为正整数，功率 P=R²。
//       可以在基线上任意实数位置加最多 K 个中继，使相邻设备间距都 ≤ R 从而连通 1..N。
//       求最小 P。
// 判定 check(R)：对每段间距 d = x[i+1]-x[i]，最少需要 ceil(d/R)-1 个中继；
//                若 Σ(ceil(d/R)-1) ≤ K 则可行。R 越大越容易 ⇒ 单调 ⇒ 二分最小的 R。
// ⚠️ 整除写法：需要中继数 = (d + R - 1)/R - 1 = (d - 1)/R （d ≥ 1 时两者相等，且不会出现 d=0）
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll N, K;
    if (!(cin >> N >> K)) return 0;
    vector<ll> x(N);
    for (ll i = 0; i < N; ++i) cin >> x[i];

    auto check = [&](ll R) {                       // R = 候选通信距离
        ll need = 0;
        for (ll i = 0; i + 1 < N; ++i) {
            ll d = x[i + 1] - x[i];
            if (d > R) {
                need += (d - 1) / R;               // = ceil(d/R) - 1
                if (need > K) return false;        // 早停，防溢出
            }
        }
        return need <= K;
    };

    ll lo = 1, hi = x[N - 1] - x[0];               // R 最小 1；最大不超过首尾间距（一定可行）
    while (lo < hi) {
        ll mid = lo + (hi - lo) / 2;
        if (check(mid)) hi = mid; else lo = mid + 1;
    }
    cout << lo * lo << "\n";                       // P = R²
    return 0;
}
