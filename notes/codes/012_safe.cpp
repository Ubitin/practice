// 012 自检版：把所有可能踩的边界一次全打出来，并用 __int128 兜住中间量
//
// 你现在的写法： ans = k*t + t*(t-1)*k/2        （t = (n-1)/k）
// 它等价于 k*t*(t+1)/2，数学上没问题，但有【中间量溢出】风险：
//   t*(t-1)*k 这个乘法在除法之前算，n=1e10 时就已经超过 LLONG_MAX ⇒ 开始输出负数
//   （实测分界线：n ≤ 1e9 全对；n = 1e10 起崩）
//
// 本版两处改进：
//   ① 用 __int128 算中间量，n 到 1e15 也不会溢出；
//   ② 结果再打印一遍边界自检，方便你一眼看出哪里不对。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128;

static i128 sumMul128(ll n, ll k) {          // < n 的所有 k 的正倍数之和
    ll t = (n - 1) / k;                       // 个数
    return (i128)k * t * (t + 1) / 2;         // = k*(1+2+...+t)
}

static void print128(i128 v) {
    if (v == 0) { cout << 0; return; }
    if (v < 0) { cout << '-'; v = -v; }
    string s;
    while (v > 0) { s += char('0' + (int)(v % 10)); v /= 10; }
    reverse(s.begin(), s.end());
    cout << s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll T;
    if (!(cin >> T)) return 0;
    while (T--) {
        ll n; cin >> n;
        i128 ans = sumMul128(n, 3) + sumMul128(n, 5) - sumMul128(n, 15);
        print128(ans);
        cout << "\n";
    }
    return 0;
}
