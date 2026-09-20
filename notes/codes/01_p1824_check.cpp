// ============================================================
//  01_p1824_check.cpp —— 复现原码两个 bug + 验证修正版
//
//  原文件 test_/01.cpp 保持原样、未做任何改动。
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ---------- [1] 变量遮蔽：复刻原码结构，看它读到什么 ----------
static void demo_shadow() {
    cout << "===== [1] 变量遮蔽实况复刻 =====\n";

    // 全局作用域（模拟 01.cpp 第 4 行）
    static vector<ll> g_a;                       // 空 vector，等价于全局 a

    // main 里又声明了一个同名局部变量（模拟第 38 行）
    vector<ll> a = {1, 2, 4, 8, 9};              // 局部 a：有数据

    // p() 看到的是全局那个（模拟第 7~17 行）
    auto p_sees_global = [&]() -> size_t { return g_a.size(); };
    auto p_sees_local  = [&]() -> size_t { return a.size(); };

    cout << "  局部 a（main 里读取+排序用的）: size = " << p_sees_local()  << "\n";
    cout << "  全局 a（p() 实际读到的）      : size = " << p_sees_global() << "\n";
    cout << "  → p() 里 a[0] 访问的是【空 vector】的第 0 个元素\n";
    cout << "  → 空 vector 的 data() 通常是 nullptr，*(nullptr) = 解引用空指针\n";
    cout << "  实测原码退出码 = -1073741819 = 0x00000005 高位 → ACCESS_VIOLATION\n\n";
}

// ---------- [2] long 在 Windows 上只有 32 位，l + r 会溢出 ----------
static void demo_long_overflow() {
    cout << "===== [2] find(long l, long r) 的溢出风险 =====\n";
    cout << "  sizeof(long)      = " << sizeof(long)      << " 字节\n";
    cout << "  sizeof(long long) = " << sizeof(long long) << " 字节\n";
    cout << "  LONG_MAX          = " << LONG_MAX << "\n\n";

    // 场景：某次 p(mid) 成功，l = mid + 1 = 1e9 + 1，而 r 仍是 2e9
    long      l32 = 1000000001L, r32 = 2000000000L;
    long long l64 = 1000000001LL, r64 = 2000000000LL;

    cout << "  模拟一次真实的中间状态：l = " << l32 << " , r = " << r32 << "\n";
    cout << "  真实和            = " << (l64 + r64) << "   (long long)\n";
    cout << "  用 32 位 long 算   = " << (long)(l32 + r32) << "   ← 溢出，结果完全不同\n";
    cout << "  是否相等： " << (((long)(l32 + r32)) == (l64 + r64) ? "是" : "否 ✘") << "\n";
    cout << "  → 之后 (l + r) >> 1 得到的 mid 是垃圾值，二分直接跑偏\n";
    cout << "  ⚠️ 洛谷是 Linux(LP64)，long 是 64 位 → 侥幸不炸；本机 Windows 会炸\n\n";
}

// ---------- [3] 修正版 vs 暴力 对拍 ----------
static ll N, C;
static vector<ll> A;

static bool ok(ll d) {
    ll cnt = 1, last = A[0];
    for (ll i = 1; i < N; ++i)
        if (A[i] - last >= d) { ++cnt; last = A[i]; }
    return cnt >= C;
}
static ll solve_fixed(vector<ll> v, ll c) {
    A = v; N = (ll)v.size(); C = c;
    sort(A.begin(), A.end());
    ll lo = 0, hi = A[N - 1] - A[0];
    while (lo < hi) {
        ll mid = lo + (hi - lo + 1) / 2;
        if (ok(mid)) lo = mid; else hi = mid - 1;
    }
    return lo;
}
// 暴力：从大到小枚举最小距离 d
static ll brute(vector<ll> v, ll c) {
    sort(v.begin(), v.end());
    ll n = v.size(), mx = v[n - 1] - v[0];
    for (ll d = mx; d >= 0; --d) {
        ll cnt = 1, last = v[0];
        for (ll i = 1; i < n; ++i)
            if (v[i] - last >= d) { ++cnt; last = v[i]; }
        if (cnt >= c) return d;
    }
    return 0;
}

int main() {
    demo_shadow();
    demo_long_overflow();

    cout << "===== [3] 修正版 vs 暴力 对拍（30000 组）=====\n";
    {
        mt19937_64 rng(20260917);
        int bad = 0;
        for (int tc = 0; tc < 30000; ++tc) {
            int n = 2 + rng() % 12;
            vector<ll> v(n);
            for (auto& x : v) x = rng() % 50;
            ll c = 2 + rng() % (n - 1);           // 2 <= c <= n
            if (solve_fixed(v, c) != brute(v, c)) bad++;
        }
        cout << "  30000 组随机数据，不一致 " << bad << " 组"
             << (bad == 0 ? "  → 修正版正确 ✔\n\n" : "  ✘\n\n");
    }

    cout << "===== [4] 修正版回归用例 =====\n";
    {
        struct { vector<ll> v; ll c; ll expect; const char* name; } T[] = {
            {{1,2,8,4,9}, 3, 3, "题面样例 (5 3)"},
            {{1,10},      2, 9, "最简 (2 2 / 1 10)"},
            {{5,5,5},     2, 0, "全同值 (3 2 / 5 5 5)"},
            {{1,2,3,4},   4, 1, "两两相邻 (4 4 / 1 2 3 4)"},
            {{0,1000000000}, 2, 1000000000LL, "大数 (2 2 / 0 1e9)  ← 就是它触发 long 溢出"},
        };
        for (auto& t : T) {
            ll got = solve_fixed(t.v, t.c);
            printf("  %s %-42s → %-12lld (期望 %lld)\n",
                   got == t.expect ? "OK " : "!! ", t.name, got, t.expect);
        }
    }
    return 0;
}
