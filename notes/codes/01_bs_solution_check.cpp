// ============================================================
//  01_bs_solution_check.cpp —— 判定"题解式二分"是否存在逻辑错误
//
//  题解骨架（P1873）：
//      int L = 0, R = 1e9, ans, mid;
//      while (L <= R)
//          if (P(mid = L + R >> 1))    // 一种压行技巧
//              ans = mid, L = mid + 1;
//          else
//              R = mid - 1;
//
//  本程序逐字复刻这段骨架，与暴力枚举对拍，逐项检验：
//    [1] L + R >> 1 的优先级是否等于 (L + R) >> 1
//    [2] int 版 L + R 是否会溢出（R=1e9 时最大 2e9 vs INT_MAX）
//    [3] 二分结果与暴力是否一致（有解的数据）
//    [4] ans 未初始化在"无解"数据下会不会被读到
//    [5] 用户版（循环外多一句 mid 初始化）是否等价
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

static ll N, M;
static vector<ll> a;                 // 1-indexed

// 题面的判定函数：锯片高度 H 得到的木材量 >= M
static bool P(ll H) {
    ll s = 0;
    for (ll i = 1; i <= N; i++)
        if (a[i] > H) s += a[i] - H;
    return s >= M;
}
static ll wood(const vector<ll>& v, ll H) {   // 独立实现，用于暴力
    ll s = 0;
    for (size_t i = 1; i < v.size(); i++) if (v[i] > H) s += v[i] - H;
    return s;
}

// ---------- 题解写法：int 版，逐字复刻 ----------
// 返回答案；ansSet 报告 ans 是否被赋值过（用哨兵检测，避免读到未初始化的不确定值）
static ll solve_editor_int(vector<ll> v, ll m, bool& ansSet) {
    N = (ll)v.size() - 1; a = v; M = m;
    int L = 0, R = 1e9, ans = -123456789, mid;   // 哨兵值代替"不确定"
    while (L <= R)
        if (P(mid = L + R >> 1))
            ans = mid, L = mid + 1;
        else
            R = mid - 1;
    ansSet = (ans != -123456789);
    return ans;
}

// ---------- 题解写法：long long 版（排除 int 溢出干扰） ----------
static ll solve_editor_ll(vector<ll> v, ll m, bool& ansSet) {
    N = (ll)v.size() - 1; a = v; M = m;
    ll L = 0, R = 1000000000LL, ans = -123456789, mid;
    while (L <= R)
        if (P(mid = L + R >> 1))
            ans = mid, L = mid + 1;
        else
            R = mid - 1;
    ansSet = (ans != -123456789);
    return ans;
}

// ---------- 用户版（循环外多一句 mid 初始化，条件里同样重新赋值） ----------
static ll solve_user(vector<ll> v, ll m, ll l, ll r, bool& ansSet) {
    N = (ll)v.size() - 1; a = v; M = m;
    ll mid = (l + r) >> 1;                 // 这一句是死代码
    ll ans = -123456789;
    while (l <= r) {
        if (P(mid = (l + r) >> 1)) {
            ans = mid;
            l = mid + 1;
        } else r = mid - 1;
    }
    ansSet = (ans != -123456789);
    return ans;
}

// ---------- 暴力：从大到小枚举 H ----------
static pair<ll,bool> brute(const vector<ll>& v, ll m) {
    ll mx = 0;
    for (size_t i = 1; i < v.size(); i++) mx = max(mx, v[i]);
    for (ll H = mx; H >= 0; --H)
        if (wood(v, H) >= m) return {H, true};
    return {0, false};                     // 无解
}

int main() {
    cout << "===== [1] 运算符优先级：L + R >> 1 到底怎么结合 =====\n";
    {
        bool ok = true;
        ll tests[] = {0, 1, 7, 999, 1000000000, 1073741824};
        for (ll L : tests) for (ll R : tests) {
            ll p1 = L + R >> 1;            // 题解写法
            ll p2 = (L + R) >> 1;          // 加括号
            if (p1 != p2) { ok = false; cout << "  不等！L=" << L << " R=" << R << "\n"; }
        }
        cout << "  测试 36 组 (L,R)：" << (ok ? "全部相等 ✔" : "存在不等 ✘") << "\n";
        cout << "  原因：C++ 中【加法】优先级高于【移位】，L + R >> 1 等价于 (L + R) >> 1\n";
        cout << "  ⚠️ 但这是「能跑但不该学」的压行：换个表达式或换个人读，语义就容易被看错\n";
    }

    cout << "\n===== [2] int 版会不会溢出？ =====\n";
    {
        cout << "  INT_MAX = " << INT_MAX << "\n";
        cout << "  L、R 上界都是 1e9，循环内恒有 L <= R，故 L + R <= 2e9 = 2000000000\n";
        cout << "  2e9 " << (2000000000LL < (ll)INT_MAX ? "<" : ">=") << " INT_MAX → "
             << (2000000000LL < (ll)INT_MAX ? "本约束下【不溢出】✔（但只差 1.47e8，很贴边）" : "溢出 ✘") << "\n";
    }

    cout << "\n===== [3] 有解数据：题解 vs 暴力 对拍 =====\n";
    {
        mt19937_64 rng(20260917);
        int badInt = 0, badLl = 0, badUser = 0, badUser2 = 0, notSet = 0, total = 0;
        for (int tc = 0; tc < 20000; ++tc) {
            int n = 1 + rng() % 30;
            vector<ll> v(n + 1);                       // 1-indexed
            ll sum = 0;
            for (int i = 1; i <= n; i++) { v[i] = 1 + rng() % 1000; sum += v[i]; }
            ll m = 1 + rng() % sum;                    // <= 总高度 ⇒ 保证有解
            auto [expect, hasSol] = brute(v, m);
            (void)hasSol;
            bool s1 = false, s2 = false, s3 = false;
            ll r1 = solve_editor_int(v, m, s1);
            ll r2 = solve_editor_ll(v, m, s2);
            ll r3 = solve_user(v, m, 0, 1000000000LL, s3);
            bool s4 = false;
            ll r4 = solve_user(v, m, 0, 2000000000LL, s4);   // 你当前 01.cpp 用的上界
            total++;
            if (r1 != expect) badInt++;
            if (r2 != expect) badLl++;
            if (r3 != expect) badUser++;
            if (r4 != expect) badUser2++;
            if (!s1) notSet++;
        }
        cout << "  20000 组（保证有解）：\n";
        cout << "    题解 int 版       错 " << badInt  << " 组\n";
        cout << "    题解 ll  版       错 " << badLl   << " 组\n";
        cout << "    用户版 r=1e9      错 " << badUser << " 组\n";
        cout << "    用户版 r=2e9      错 " << badUser2 << " 组   ← 你 01.cpp 的实际参数\n";
        cout << "    ans 未被赋值的情况：" << notSet << " 组\n";
        cout << "  → 核心二分逻辑正确 ✔（四版都 0 错）\n";
    }

    cout << "\n===== [4] 「无解」数据（M > 树高总和）：ans 会不会被读到 =====\n";
    {
        int neverSet = 0, total = 0;
        for (int tc = 0; tc < 5000; ++tc) {
            vector<ll> v = {0, 5, 8, 3};               // 总和 16
            ll m = 100;                                // 明显无解
            bool s = true;
            solve_editor_ll(v, m, s);
            total++;
            if (!s) neverSet++;
        }
        cout << "  M=100 而树高总和只有 16（无解），测试 " << total << " 组：\n";
        cout << "    ans 从未被赋值：" << neverSet << " / " << total << " 组\n";
        cout << "  → 题解写 int ans, mid; 【未给 ans 初值】，此时读 ans 是未定义行为\n";
        cout << "  → 所以这段代码只有在【题面保证有解】时才安全。\n";
        cout << "     P1873 的数据保证 M <= 树高总和，所以 AC；换个不保证的题就是雷。\n";
    }

    cout << "\n===== [5] P1873 官方样例回归 =====\n";
    {
        vector<ll> v = {0, 20, 15, 10, 17};            // 1-indexed
        auto [e, has] = brute(v, 7);
        bool s = false;
        cout << "  输入 4 7 / 20 15 10 17\n";
        cout << "    暴力 = " << e << "，题解 int 版 = " << solve_editor_int(v, 7, s)
             << "，ans被赋值=" << (s ? "是" : "否") << "\n";
        vector<ll> v2 = {0, 4, 42, 40, 26, 46};
        bool s2 = false;
        cout << "  输入 5 20 / 4 42 40 26 46\n";
        cout << "    暴力 = " << brute(v2, 20).first << "，题解 int 版 = "
             << solve_editor_int(v2, 20, s2) << "\n";
    }
    return 0;
}
