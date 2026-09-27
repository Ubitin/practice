// 012 推荐版：__int128（简单、快、n ≤ 10^18 都稳）
// 答案量级 ≈ 0.2667·n² ⇒ n=1e18 时约 2.7e35，long long 不够、__int128 够。
// 公式（等差数列 + 容斥）：
//   t(k) = (n-1)/k            ← "小于 n" 必须减 1
//   S(k) = k·t·(t+1)/2
//   ans  = S(3) + S(5) - S(15)
// ⚠️ 中间量 k·t·(t+1) 在 n=1e18 时约 1e36，必须走 __int128 再除 2 再取回。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128;

static i128 sumMul(ll n, ll k) {
    ll t = (n - 1) / k;                       // 个数
    return (i128)k * t * (t + 1) / 2;         // ★ 全程 __int128，先乘后除
}

static void print128(i128 v) {
    if (v == 0) { printf("0"); return; }
    if (v < 0) { printf("-"); v = -v; }
    char buf[64]; int p = 0;
    while (v > 0) { buf[p++] = char('0' + (int)(v % 10)); v /= 10; }
    while (p) putchar(buf[--p]);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll T;
    if (!(cin >> T)) return 0;
    while (T--) {
        ll n; cin >> n;
        print128(sumMul(n, 3) + sumMul(n, 5) - sumMul(n, 15));
        putchar('\n');
    }
    return 0;
}
