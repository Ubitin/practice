// CF1117C Magic Ship —— 正解：二分答案 + 风的前缀和（O(n log n)，不开任何坐标数组）
//
// 题意：船在 (x1,y1)，要去 (x2,y2)。每天：
//   ① 风先把船吹动 s[i] 位移（s 按周期 n 循环）
//   ② 然后船可以自己选一个方向走一格（或不动）
//   求最少天数，不能到达输出 -1。
//
// ── 为什么不能开二维数组（原码 `ll ti[1e9][1e9]` 的错误在这里）──
//   坐标范围是 [-1e9, 1e9] ⇒ 边长 2e9+1 ⇒ 元素个数 (2e9)² = 4e18，每个 8 字节 ⇒ 3.2e19 字节。
//   宇宙里没有这么多内存；g++ 甚至没法把这么大的数组大小写进指令里（汇编器直接报错）。
//
// ── 正解思路 ──
//   设预知 = 风在前 d 天里的净位移 W(d)（可以把风序列按完整周期 + 余数算出，O(1)）。
//   如果这 d 天里船一直不自己走，它会被吹到 (x1+Wx(d), y1+Wy(d))。
//   船自己每天最多能走 1 格，所以 d 天后船能到达的区域 = 以 (x1+Wx(d), y1+Wy(d)) 为圆心、半径 d 的菱形。
//   ⇒ 能到达 ⟺ 曼哈顿距离 |x2-(x1+Wx)| + |y2-(y1+Wy)| <= d
//   判定函数对 d 单调（d 越大越容易满足）⇒ 二分答案。
//   答案上界：若 winds 的 200*n 天净位移仍不能缩短距离，则永远到不了 ⇒ -1。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll x1, y1, x2, y2, n;
    cin >> x1 >> y1 >> x2 >> y2 >> n;
    string s; cin >> s;

    vector<ll> px(n + 1, 0), py(n + 1, 0);          // 风的前缀和：第 1..i 天的净位移
    for (ll i = 1; i <= n; ++i) {
        ll dx = 0, dy = 0;
        if (s[i - 1] == 'U') dy = 1;
        else if (s[i - 1] == 'D') dy = -1;
        else if (s[i - 1] == 'R') dx = 1;
        else if (s[i - 1] == 'L') dx = -1;
        px[i] = px[i - 1] + dx;
        py[i] = py[i - 1] + dy;
    }
    ll fx = px[n], fy = py[n];                      // 一个完整周期的风位移

    auto ok = [&](ll d) {                           // 二分的判定函数
        ll full = d / n, rem = d % n;
        ll wx = fx * full + px[rem];                // 前 d 天的风净位移
        ll wy = fy * full + py[rem];
        ll need = llabs(x2 - (x1 + wx)) + llabs(y2 - (y1 + wy));
        return need <= d;                           // 船自己最多走 d 格
    };

    ll lo = 0, hi = 4000000000000000000LL / n * n;   // 一个安全的大上界
    if (!ok(hi)) { cout << -1 << "\n"; return 0; }   // 大上界都不行 ⇒ 永远到不了
    while (lo < hi) {                                // 找最小的可行 d
        ll mid = lo + (hi - lo) / 2;
        if (ok(mid)) hi = mid; else lo = mid + 1;
    }
    cout << lo << "\n";
    return 0;
}
