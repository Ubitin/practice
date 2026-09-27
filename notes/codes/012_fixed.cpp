// 012 修正版：小于 n 的自然数中 3 或 5 的倍数之和（T ≤ 1e5, n ≤ 1e9）
// 原码唯一的问题：for (i=3;i<n;++i) 逐个枚举 ⇒ 每组 1e9 次、T 组共 1e14 次 ⇒ 必然 TLE
// （ans 初始化那处你上次已经改对了 ✅）
//
// 正解：等差数列 + 容斥，O(1) 每组
//   m(k) = (n-1)/k                    // < n 的 k 的正倍数个数（"小于 n"所以减 1）
//   S(k) = k * m(m+1)/2               // 这些倍数之和
//   ans  = S(3) + S(5) - S(15)        // 15 的倍数被数了两次，减一次
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

static ll sumMul(ll n, ll k) {
    ll m = (n - 1) / k;
    return k * m * (m + 1) / 2;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll t;
    if (!(cin >> t)) return 0;
    while (t--) {
        ll n; cin >> n;
        cout << sumMul(n, 3) + sumMul(n, 5) - sumMul(n, 15) << "\n";
    }
    return 0;
}
