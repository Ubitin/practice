// 对比：试除法 gcd  vs  辗转相除 gcd（第 9 行那种）
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 你的原写法（试除）
ll gcd_trial(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    ll r = min(a, b);
    for (ll i = r; i >= 2; --i)
        if (a % i == 0 && b % i == 0) return i;
    return 1;
}

// 第 9 行的写法（辗转相除）
ll gcd_euclid(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

int main() {
    // 1) 正确性对比（含负数）
    struct C { ll a, b; };
    vector<C> cs = {{12,8},{21,14},{1,2},{0,5},{-12,8},{12,-8},{-12,-8},{1000000007,998244353},
                    {999999999999999989LL, 1000000000000000000LL}};
    printf("正确性：\n");
    for (auto& c : cs) {
        ll e = gcd_euclid(c.a, c.b);
        printf("  gcd(%lld, %lld) = %lld   （Python 真值见脚本核对）\n", c.a, c.b, e);
    }
    // 负数时试除法（你的原写法）会怎样
    printf("\n你的试除法在负数上的表现：\n");
    for (auto& c : cs) {
        if (c.a < 0 || c.b < 0)
            printf("  gcd_trial(%lld, %lld) = %lld   ← 注意与辗转相除是否一致\n",
                   c.a, c.b, gcd_trial(c.a, c.b));
    }

    // 2) 速度对比：用一对大且互素的数
    ll A = 999999999999999989LL, B = 1000000000000000000LL;
    auto t0 = chrono::steady_clock::now();
    ll r1 = gcd_euclid(A, B);
    auto t1 = chrono::steady_clock::now();
    printf("\n辗转相除 gcd(%lld, %lld) = %lld，耗时 %.6f ms\n", A, B, r1,
           chrono::duration<double, milli>(t1 - t0).count());

    // 试除法只用在"小一点的数"上演示，否则要跑几百年
    ll S = 100000000LL;   // 1e8
    auto t2 = chrono::steady_clock::now();
    ll r2 = gcd_trial(S - 1, S);          // 互素，试除法要退到 i=1 才返回
    auto t3 = chrono::steady_clock::now();
    double ms = chrono::duration<double, milli>(t3 - t2).count();
    printf("试除法   gcd(%lld, %lld) = %lld，耗时 %.1f ms", S - 1, S, r2, ms);
    printf("   ⇒ 外推到 1e18 规模约 %.3e ms（约 %.2e 年）\n", ms * 1e10, ms * 1e10 / 1000 / 3600 / 24 / 365);

    auto t4 = chrono::steady_clock::now();
    ll r3 = gcd_euclid(S - 1, S);
    auto t5 = chrono::steady_clock::now();
    printf("辗转相除 gcd(%lld, %lld) = %lld，耗时 %.6f ms\n", S - 1, S, r3,
           chrono::duration<double, milli>(t5 - t4).count());

    // 3) 顺带：std::gcd 一行就够（C++17，<numeric>）
    printf("\nstd::gcd(%lld, %lld) = %lld   ← C++17 标准库已有，一行顶上面整段\n",
           A, B, (ll)std::gcd(A, B));
    printf("std::gcd(-12, 8) = %lld   ← 它自己会处理符号（返回非负）\n", (ll)std::gcd((ll)-12, (ll)8));
    return 0;
}
