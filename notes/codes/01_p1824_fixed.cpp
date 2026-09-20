// ============================================================
//  01_p1824_fixed.cpp —— P1824 进击的奶牛（修正版）
//
//  原文件 test_/01.cpp 保持原样、未做任何改动。
//
//  题意：n 个牛棚在数轴上（位置 a[i]），放 c 头牛，
//        使任意两头牛之间的最小距离尽可能大，求这个最大的最小距离。
//
//  ── 原码的致命 bug：变量遮蔽（shadowing）──
//    第 4 行  vector<long long> a;          ← 全局 a（空的）
//    第 38 行 vector<long long> a(n, 0);    ← main 里的【局部】a，遮蔽了全局
//    main 把数据读进并排序的是【局部 a】，
//    而 p() 用的是【全局 a】——它始终是空的，
//    于是 a[0] 对空 vector 取下标 = 解引用空指针 → 0xC0000005 访问违规 → 崩溃。
//    实测：退出码 -1073741819；g++ -Wshadow 会直接给出上面那条警告。
//
//  ── 第二个隐藏 bug：find 的形参用了 long ──
//    第 19 行 long long find(long l, long r)
//    Windows(MinGW-w64) 是 LLP64：long 只有 32 位（LONG_MAX = 2147483647）。
//    而 main 传的 r = 2e9；当 l 涨到 1e9+1 时 l + r = 3e9 → 【32 位 long 溢出】→ UB。
//    （洛谷是 Linux/LP64，long 是 64 位，所以在洛谷上侥幸不炸；本机 Windows 会炸。）
//    修正：一律 long long。
//
//  ── 其余做得对的地方 ──
//    · p() 的贪心正确：第一头放最左，之后每头放在第一个 >= last + d 的位置
//    · 二分的是【值域】（最小距离），不是数组下标 ✔
//    · 排序了 ✔（贪心必须基于有序位置）
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll n, c;
vector<ll> a;                 // ★ 只保留这一个 a，main 里不再重复声明

// 最小间距为 d 时，最多能放下几头牛
bool ok(ll d) {
    ll cnt = 1, last = a[0];  // 第一头牛放在最左边的牛棚
    for (ll i = 1; i < n; ++i) {
        if (a[i] - last >= d) {
            ++cnt;
            last = a[i];      // 贪心：能放就放，放得越靠左后面越有余地
        }
        if (cnt >= c) return true;   // 提前返回，省时间
    }
    return cnt >= c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> c;
    a.resize(n);                       // ★ 用全局 a
    for (ll i = 0; i < n; ++i) cin >> a[i];
    sort(a.begin(), a.end());

    ll lo = 0, hi = a[n - 1] - a[0];   // ★ 答案不会超过「最远两个牛棚的距离」
    while (lo < hi) {
        ll mid = lo + (hi - lo + 1) / 2;   // 上取整（配 lo = mid）
        if (ok(mid)) lo = mid;             // 可行 → 试更大的距离
        else         hi = mid - 1;
    }
    cout << lo << '\n';
    return 0;
}
