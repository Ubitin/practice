// 013 正解：a * b mod m，1 ≤ a,b,m ≤ 10^18
//
// 【要不要高精度？不需要。用 __int128 就够，原因是量级算得清】
//   a,b ≤ 1e18 ⇒ a*b ≤ 1e36
//     long long       上限 9.22e18   ← 装不下（差 17 个数量级）
//     unsigned long long 上限 1.84e19 ← 也装不下
//     __int128        上限 ≈1.70e38  ← ✅ 装得下，还余 2 个数量级的空间
//   而且【取模之后结果 < m ≤ 1e18】，永远不会超 ⇒ 全程 __int128 就够，不必写大数。
//
// 【判据（记住这一条就够了）】
//   把"最大中间量"估出来，和类型上限比：
//     ≤ 9.2e18  → long long
//     ≤ 1.7e38  → __int128
//     > 1.7e38  → 才需要高精度（如 a*b mod m 里 a,b 到 1e19 以上）
#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;
using ull128 = unsigned __int128;

static void printU128(ull128 v) {
    if (v == 0) { putchar('0'); return; }
    char buf[48]; int p = 0;
    while (v > 0) { buf[p++] = char('0' + (int)(v % 10)); v /= 10; }
    while (p) putchar(buf[--p]);
}

int main() {
    ull a, b, m;
    if (!(cin >> a >> b >> m)) return 0;
    ull128 prod = (ull128)a * b;     // ★ 关键：先升到 128 位再乘
    printU128(prod % m);
    putchar('\n');
    return 0;
}
