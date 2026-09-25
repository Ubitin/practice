// 小于 n 的自然数中 3 或 5 的倍数之和 —— 公式解 + 与暴力解全量对照
//
// 公式（容斥原理）：
//   m(k) = (n-1)/k                       // < n 的 k 的倍数个数 = 0, k, 2k, ..., m*k 中非零的个数
//   S(k) = k * m(m+1)/2                  // k+2k+...+m*k
//   ans  = S(3) + S(5) - S(15)           // 3 和 5 的倍数并集（15 的倍数被数了两次要减掉）
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

static ll S(ll n, ll k) {          // 所有 < n 的 k 的正倍数之和
    ll m = (n - 1) / k;
    return (ll)k * m * (m + 1) / 2;
}
static ll formula(ll n) { return S(n, 3) + S(n, 5) - S(n, 15); }

static ll brute(ll n) {            // 直接枚举，只用于小 n 验证
    ll s = 0;
    for (ll i = 3; i < n; ++i) if (i % 3 == 0 || i % 5 == 0) s += i;
    return s;
}

int main() {
    // 1) 样例
    printf("n=10  公式=%lld （样例 23）\n", formula(10));
    printf("n=100 公式=%lld （样例 2318）\n", formula(100));

    // 2) 全量对照：n = 1..200000
    ll bad = 0, firstBad = -1;
    for (ll n = 1; n <= 200000; ++n) {
        if (formula(n) != brute(n)) { if (firstBad < 0) firstBad = n; ++bad; }
    }
    printf("n=1..200000 公式 vs 暴力：不一致 %lld 个%s\n", bad,
           bad ? "" : " ✅");

    // 3) 抽几组大 n 打印（观察量级）
    ll tests[] = {1000000000LL, 999999999LL, 1000000007LL};
    for (ll n : tests) printf("n=%lld -> %lld\n", n, formula(n));
    return 0;
}
