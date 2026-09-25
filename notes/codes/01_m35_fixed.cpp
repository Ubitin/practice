// 小于 n 的自然数中 3 或 5 的倍数之和 —— 修正版
//
// 原码 test_\01.cpp 的两个问题：
//   ① ans 是全局变量、在 while(t--) 外面，多组数据【不清零】⇒ 第 2 组答案是"前两组之和"
//   ② 用 for(i=3;i<n;++i) 逐个枚举 ⇒ T=1e5、n=1e9 时是 1e14 次循环，必然 TLE
//
// 本版：每组重新计算 + 容斥公式 O(1)
//   S(k) = k * m(m+1)/2，其中 m = (n-1)/k（< n 的 k 的正倍数个数）
//   ans = S(3) + S(5) - S(15)      ← 15 的倍数（3 和 5 的公倍数）被数了两次，减掉一次
//
// ⚠️ 本文件是新建的，未改动你的 test_\01.cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll sumMul(ll n, ll k) {          // 所有 < n 的 k 的正倍数之和
    ll m = (n - 1) / k;          // 个数：k, 2k, ..., m*k 都 < n
    return (ll)k * m * (m + 1) / 2;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll t;
    if (!(cin >> t)) return 0;
    while (t--) {
        ll n;
        cin >> n;
        ll ans = sumMul(n, 3) + sumMul(n, 5) - sumMul(n, 15);   // ★ 每组重新算，不跨组累计
        cout << ans << '\n';
    }
    return 0;
}
