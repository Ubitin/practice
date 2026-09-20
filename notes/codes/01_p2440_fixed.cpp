// ============================================================
//  01_p2440_fixed.cpp —— P2440 木材加工（修正版）
//
//  原文件 test_/01.cpp 保持原样、未做任何改动。
//
//  题意：n 段原木，长度 len[i]，要切出 k 段等长小段，求最大的整数长度；
//        连长度 1 都切不出 k 段则输出 0。
//
//  ── 原码 RE 的原因：整数除零 ──
//    main 里写的是  long long l = 0, r = 1e9;
//    find() 里每一次都算 mid = (l + r) >> 1，只要 l 仍是 0 而 r 缩到 1，
//    mid 就会取到 0，于是 p(0) 里执行 len[i] / 0
//    → 整数除零 → 实测退出码 -1073741676 = 0xC0000094（STATUS_INTEGER_DIVIDE_BY_ZERO）
//    什么时候会把 r 缩到 1？—— 当【答案是 1】时：p(1) 为真但 p(2) 为假，
//    搜索路径会走到 l=0, r=5 → mid=2 → 假 → r=1 → mid=0 → 崩溃。
//    实测：n=1,k=2,len={3} 与 n=2,k=3,len={5,1} 都崩；题面样例（答案 114）不崩。
//
//  ── 修法（两条都要）──
//    ① 二分下界从 1 起（长度 0 没有意义，还必然除零）：lo = 1
//    ② p() 里对 x <= 0 兜底返回 false（双保险，防止以后改代码又踩回来）
//  另外：上界用 max(len)，比 1e9 少跑几轮；p() 里加提前返回。
//
//  ── 原码做得对的地方 ──
//    · find 的形参是 long long（没有重蹈 P1824 的 long 覆辙）
//    · len 是全局且用 resize，没有变量遮蔽
//    · p() 的判据 res >= k 与 ∑⌊len[i]/x⌋ 的模型正确
//    · 排除了 sum < k（输出 0）与 sum == k（输出 1）两种边界
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll n, k;
vector<ll> len;

// 每段长 x 时能否切出至少 k 段
bool ok(ll x) {
    if (x <= 0) return false;          // ★ 兜底：x == 0 会整数除零
    ll res = 0;
    for (ll i = 0; i < n; ++i) {
        res += len[i] / x;
        if (res >= k) return true;     // 提前返回：既省时间也防 res 溢出
    }
    return res >= k;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> k;
    len.resize(n);
    ll mx = 0;
    for (ll i = 0; i < n; ++i) {
        cin >> len[i];
        mx = max(mx, len[i]);
    }

    ll lo = 1, hi = mx;                // ★ 下界是 1，绝不可能取到 0
    ll ans = 0;                        // 切不出 k 段时保持 0
    while (lo <= hi) {
        ll mid = (lo + hi) >> 1;
        if (ok(mid)) { ans = mid; lo = mid + 1; }
        else         { hi = mid - 1; }
    }
    cout << ans << '\n';
    return 0;
}
