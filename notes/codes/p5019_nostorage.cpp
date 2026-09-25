// P5019 铺设道路 —— 不存数组版（边读边算）
//
// 原题解 test_\p5019.cpp 的做法是先存下全部 a[i]，再从头扫一遍求
//   ans = a[0] + Σ max(0, a[i] - a[i-1])
// 本版把它统一成：令"第 1 段的前一段高度 = 0"，则整个公式变成
//   ans = Σ max(0, a[i] - a[i-1])      （i 从 1 到 n，a[0] 视作 0）
// 因为"从 0 升到 a[0]"本身就是一次上升 —— 最左端和上坡是同一件事。
//
// 好处：① 不需要数组（O(1) 额外空间）；② 天然不存在 a[0] 在 n=0 时越界的问题。
//
// ⚠️ 本文件是新建的，未改动你的 test_\p5019.cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n;
    if (!(cin >> n)) return 0;

    ll ans = 0;        // 累计所有"上升量"
    ll prev = 0;       // "上一段"的高度；第 1 段的上一段视作 0
    for (ll i = 0; i < n; ++i) {
        ll x;
        cin >> x;
        if (x > prev) ans += x - prev;   // 上坡（含"从 0 升到 a[0]"）才计入
        prev = x;
    }
    cout << ans << '\n';                 // 用 '\n' 而非 endl，避免多余 flush
    return 0;
}
