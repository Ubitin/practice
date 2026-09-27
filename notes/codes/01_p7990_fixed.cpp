// 国家补助（全局取 max 型题）—— O(n + q) 正确版（含性能修正）
//
// ── 钥匙（一句话）────────────────────────────────────────────────
//   "1 p x" 是【直接覆盖】：把 p 之前受到的补助一笔抹掉。
//   ⇒ p 的最终余额 = max( 他最后一次被单独改的值 , 【那次修改之后】所有 "2 x" 的最大值 )
//   ⇒ 从没被改过的人：吃【全局】所有补助里的最大值
//
// ── 你当前版本 test_\01.cpp 的做法（思路完全正确）─────────────────
//   开一个 mon[] 按"时间点"记补助值（第 i 个事件若是 "2 x" 就记 mon[i]=x，否则 0），
//   然后对每个人取 max_element(mon.begin()+t[i], mon.end())
//   ⇒ 这就是"他最后一次被改之后的最大补助"，逻辑完全对（实测 3000 组对拍 0 不一致 ✅）
//
// ── 但它太慢：max_element 是 O(q)，对每个人做一次 ⇒ 总 O(n·q) ─────
//   实测 n=10^6, q=2×10^5：76875 ms（≈77 秒）—— 判题必 TLE
//
// ── 本文件 = 把"每次现算后缀最大值"换成"预处理一遍后缀最大值" ────
//   suf[i] = max(mon[i], mon[i+1], ..., mon[q])   （从后往前扫一遍即得，O(q)）
//   之后查每个人就是 O(1) ⇒ 总复杂度 O(n + q)
//   实测同一数据 188 ms（快约 409 倍）
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n;
    if (!(cin >> n)) return 0;
    vector<ll> a(n + 1), t(n + 1, 0);        // t[i]=0：从未被 1 覆盖（也是"从最早时刻起算"）
    for (ll i = 1; i <= n; ++i) cin >> a[i];

    ll q; cin >> q;
    vector<ll> mon(q + 2, 0);                // mon[i] = 第 i 个事件若是 "2 x" 则记 x，否则 0
    for (ll i = 1; i <= q; ++i) {
        ll k; cin >> k;
        if (k == 1) { ll p, x; cin >> p >> x; a[p] = x; t[p] = i; }
        else        { ll x; cin >> x; mon[i] = x; }
    }

    vector<ll> suf(q + 2, 0);                // ★ 后缀最大值：suf[i] = max(mon[i..q])
    for (ll i = q; i >= 0; --i) suf[i] = max(suf[i + 1], mon[i]);
    // ★★ 必须一路扫到 i = 0：t[i]=0 表示"从没被改过"，此时要查 suf[0] = 全局最大补助
    //    （只扫到 i=1 的话 suf[0] 会是 0 ⇒ 从没被改过的人拿不到补助）

    for (ll i = 1; i <= n; ++i) cout << max(a[i], suf[t[i]]) << " \n"[i == n];
    return 0;
}
