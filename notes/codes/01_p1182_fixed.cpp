// ============================================================
//  01_p1182_fixed.cpp —— P1182 数列分段 Section II（正解）
//
//  原文件 test_/01.cpp 保持原样、未做任何改动。
//
//  题意：把 n 个正整数分成 m 段（连续），使【每段和的最大值】最小，求这个最小值。
//
//  二分答案 x = 每段和的上限：
//    check(x)：贪心地从左到右累加，装不下就另起一段，统计【最少需要几段】cnt
//              若 cnt <= m 则可行（因为每个元素都 <= x 时，总能再往细里拆到恰好 m 段）
//    下界 lo = max(a[i])   ← 关键！每段至少含一个数，上限不可能小于最大元素
//    上界 hi = sum(a[i])   ← 只有一个元素时必然可行
//    求【最小可行值】：mid = lo + (hi-lo)/2 下取整，配 hi = mid
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, m;
    cin >> n >> m;
    vector<ll> a(n + 1);
    ll lo = 0, hi = 0;
    for (ll i = 1; i <= n; ++i) {
        cin >> a[i];
        lo = max(lo, a[i]);        // ★ 下界是最大元素，不是 0
        hi += a[i];                // ★ 上界是总和（可达 1e5 * 1e8 = 1e13，必须 long long）
    }

    auto ok = [&](ll x) -> bool {
        ll cnt = 1, cur = 0;
        for (ll i = 1; i <= n; ++i) {
            if (cur + a[i] <= x) cur += a[i];
            else { ++cnt; cur = a[i]; }   // 装不下，另起一段
        }
        return cnt <= m;                  // ★ 最少段数不超过 m 即可行
    };

    while (lo < hi) {
        ll mid = lo + (hi - lo) / 2;      // 下取整，配 hi = mid
        if (ok(mid)) hi = mid;            // 可行 → 试更小
        else         lo = mid + 1;
    }
    cout << lo << '\n';
    return 0;
}
