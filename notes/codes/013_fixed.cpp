// 013 修正版：a^b mod m，1 ≤ a,b,m ≤ 2^63
//
// 原码为什么错（两层）：
//   ① 【做法错】pow(a,b) 求的是【浮点数】幂，返回 double。double 只有 53 位有效位，
//      幂一旦超过约 2^53 就丢精度；超过 1.8e308 直接变成 inf ⇒ temp 转 long long 是 UB
//      （实测 2^63 → 输出负数 -291172004）。正确做法是【快速幂】边乘边取模，全程整数。
//   ② 【隐藏坑】即使换成整数快速幂，`a * b` 在 a,b 都到 2^63 时也会溢出 long long
//      （两个 2^63 相乘 ≈ 1.7e38 ≫ LLONG_MAX ≈ 9.2e18）⇒ 乘法必须用 unsigned long long
//      或 __int128 中转。
//
// 快速幂：把指数 b 拆成二进制，边平方边乘，每次乘法后取模 ⇒ O(log b)，约 63 次迭代
#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;

ull mulmod(ull a, ull b, ull m) {          // (a*b) % m，用 __int128 避免溢出
    return (ull)((__int128)a * b % m);
}

ull power(ull a, ull b, ull m) {           // a^b mod m
    ull res = 1 % m;                       // ⚠️ m 可能为 1，此时结果必须是 0
    a %= m;
    while (b) {
        if (b & 1ULL) res = mulmod(res, a, m);
        a = mulmod(a, a, m);
        b >>= 1;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ull a, b, m;
    cin >> a >> b >> m;
    cout << power(a, b, m) << "\n";
    return 0;
}
