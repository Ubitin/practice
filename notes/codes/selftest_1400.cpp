// 自测用参考实现（供你对照，不是让你抄）
//   A. ≤1000 档：网格导航模拟（典型 Div.3 A/B）
//   B. 1400 档 ①：等值对二分（排序 + lower_bound，典型 C/D）
//   C. 1400 档 ②：容器灌水（二分答案，典型 D）
//   D. 1400 档 ③：区间和奇偶查询（前缀和，典型 C/D）
// 用法：每道题都有 main 里的分节，编译后按输入节自己喂数据
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ───────── B. 等值对二分：排序后对每个 a[i] 找 (−a[i]) 的出现次数 ─────────
int solveB() {
    ll n; if (!(cin >> n)) return -1;
    vector<ll> a(n);
    for (ll i = 0; i < n; ++i) cin >> a[i];
    sort(a.begin(), a.end());
    ll cnt = 0;
    for (ll i = 0; i < n; ++i) {
        if (a[i] > 0) break;                        // 只数 a[i] <= 0 的一侧，避免重复
        auto lo = lower_bound(a.begin(), a.end(), -a[i]);
        auto hi = upper_bound(a.begin(), a.end(), -a[i]);
        ll k = hi - lo;
        if (-a[i] == a[i]) cnt += k * (k - 1) / 2;  // 0 与 0 配对（自己不能和自己配）
        else cnt += k;                              // a[i] 与值为 -a[i] 的每个数配对
    }
    cout << cnt << "\n";
    return 0;
}

// ───────── C. 容器灌水（二分答案）：最大化最小水位 ─────────
// 判定 x 可行 ⟺ Σ max(0, x - a[i]) ≤ w
int solveC() {
    ll t; if (!(cin >> t)) return -1;
    while (t--) {
        ll n, w; cin >> n >> w;
        vector<ll> a(n);
        for (ll i = 0; i < n; ++i) cin >> a[i];
        ll lo = 0, hi = 2000000000LL;               // 上界给足（w ≤ 1e18，水位最大 ≈1e9+1e9/n）
        while (lo < hi) {
            ll mid = lo + (hi - lo + 1) / 2;        // 上取整，防死循环
            __int128 need = 0;
            for (ll i = 0; i < n; ++i) if (a[i] < mid) need += (__int128)mid - a[i];
            if (need <= (__int128)w) lo = mid; else hi = mid - 1;
        }
        cout << lo << "\n";
    }
    return 0;
}

// ───────── D. 区间和奇偶查询（前缀和） ─────────
// 问 [l,r] 的和的奇偶：等价于 (pre[r]-pre[l-1]) 的奇偶
int solveD() {
    ll n, q; if (!(cin >> n >> q)) return -1;
    vector<ll> pre(n + 1, 0);
    for (ll i = 1; i <= n; ++i) { ll x; cin >> x; pre[i] = pre[i - 1] + x; }
    while (q--) {
        ll l, r, k; cin >> l >> r >> k;
        ll oddCnt = r - l + 1;                      // 区间长度
        ll sumOdd = (pre[r] - pre[l - 1]) & 1LL;
        // 把这一段的奇偶改掉后，总和奇偶 = 原奇偶变化（这里按题面各不同，仅示范写法）
        cout << ((sumOdd ^ (oddCnt & 1LL & (k & 1LL))) ? "YES" : "NO") << "\n";
    }
    return 0;
}

int main(int argc, char** argv) {
    int which = (argc > 1) ? atoi(argv[1]) : 1;
    if (which == 1) return solveB();
    if (which == 2) return solveC();
    if (which == 3) return solveD();
    return 0;
}
