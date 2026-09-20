// ============================================================
//  01_p2440_check.cpp —— P2440：定位"整数除零 RE"并验证修正版
//
//  原文件 test_/01.cpp 保持原样、未做任何改动。
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

static ll N, K;
static vector<ll> L;

static bool p_orig(ll x) {                 // 原码的判定：没有除零保护
    ll res = 0;
    for (ll i = 0; i < N; ++i) res += (L[i] / x);
    return res >= K;
}

// ---------- 复刻原码 find()，但把 mid==0 拦下来并记录轨迹 ----------
static ll find_traced(vector<ll> len, ll k, ll l, ll r, int& zeroMidStep, vector<string>& trace) {
    L = len; N = (ll)len.size(); K = k;
    ll mid = (l + r) >> 1;
    ll ans = 0;
    int step = 0;
    zeroMidStep = -1;
    while (l <= r) {
        mid = (l + r) >> 1;
        ++step;
        if (mid == 0) {                    // ← 原码这里就是 len[i] / 0 → 崩溃
            zeroMidStep = step;
            char buf[128];
            snprintf(buf, sizeof buf, "第 %2d 步: l=%lld r=%lld  mid=0  ← 即将执行 len[i]/0 【崩溃】", step, l, r);
            trace.push_back(buf);
            return ans;
        }
        bool res = p_orig(mid);
        char buf[128];
        snprintf(buf, sizeof buf, "第 %2d 步: l=%-11lld r=%-11lld mid=%-11lld p=%d", step, l, r, mid, (int)res);
        trace.push_back(buf);
        if (res) { ans = mid; l = mid + 1; }
        else r = mid - 1;
    }
    return ans;
}

// ---------- 修正版 ----------
static bool ok_fixed(ll x) {
    if (x <= 0) return false;
    ll res = 0;
    for (ll i = 0; i < N; ++i) { res += L[i] / x; if (res >= K) return true; }
    return res >= K;
}
static ll solve_fixed(vector<ll> len, ll k) {
    L = len;                                   // ← 之前漏了这句，ok_fixed 读的是残留数据
    N = (ll)len.size(); K = k;
    ll mx = 0; for (ll x : len) mx = max(mx, x);
    ll lo = 1, hi = mx, ans = 0;
    while (lo <= hi) {
        ll mid = (lo + hi) >> 1;
        if (ok_fixed(mid)) { ans = mid; lo = mid + 1; }
        else hi = mid - 1;
    }
    return ans;
}
// 暴力：从大到小枚举长度
static ll brute(vector<ll> len, ll k) {
    ll mx = 0; for (ll x : len) mx = max(mx, x);
    for (ll d = mx; d >= 1; --d) {
        ll s = 0; for (ll x : len) s += x / d;
        if (s >= k) return d;
    }
    return 0;
}

int main() {
    cout << "===== [1] 原码 RE 的完整轨迹：n=1, k=2, len={3}（期望答案 1）=====\n";
    {
        int zs = -1; vector<string> tr;
        ll a = find_traced({3}, 2, 0, 1000000000LL, zs, tr);
        printf("  搜索从 l=0, r=1e9 开始，共走了 %zu 步：\n", tr.size());
        size_t start = tr.size() > 6 ? tr.size() - 6 : 0;
        if (start > 0) printf("  ...（前面 %zu 步省略）\n", start);
        for (size_t i = start; i < tr.size(); ++i) printf("  %s\n", tr[i].c_str());
        printf("  → 第 %d 步 mid 变成 0，原码就在这一句执行 len[i] / 0 → RE\n", zs);
        printf("  → 实测退出码 -1073741676 = 0xC0000094 = STATUS_INTEGER_DIVIDE_BY_ZERO\n");
    }

    cout << "\n===== [2] 为什么「答案=1」必然踩到：r 的收缩序列 =====\n";
    {
        printf("  l 恒为 0 时，r 每次变成 (r>>1)-1：\n  1e9");
        ll r = 1000000000LL;
        int cnt = 0;
        while (r > 1 && cnt < 100) { r = (r >> 1) - 1; ++cnt; if (cnt % 4 == 0 || r <= 6) printf(" → %lld", r); }
        printf("\n  → r 缩到 5 时 mid=2；若 p(2) 为假（答案就是 1），r 变成 1\n");
        printf("  → 下一轮 l=0, r=1 → mid=0 → 崩溃\n");
        printf("  ⚠️ 只要在 l=0 一路为假，r 迟早会经过 5/4 这一带 —— 所以「答案=1」几乎必然崩\n");
    }

    cout << "\n===== [3] 修正版回归用例 =====\n";
    {
        struct { vector<ll> len; ll k; ll expect; const char* name; } T[] = {
            {{232,124,456}, 7,   114,        "题面样例 (3 7)"},
            {{3},           2,   1,          "答案=1 (1 2 / 3)      ← 原码崩在这"},
            {{5,1},         3,   1,          "答案=1 (2 3 / 5 1)    ← 原码崩在这"},
            {{3},         100,   0,          "无解   (1 100 / 3)"},
            {{3},           3,   1,          "sum==k (1 3 / 3)"},
            {{100000000,100000000}, 100, 2000000, "大数 (2 100 / 1e8 1e8)"},
            {{1,1,1},       3,   1,          "全 1   (3 3 / 1 1 1)"},
        };
        for (auto& t : T) {
            ll got = solve_fixed(t.len, t.k);
            printf("  %s %-38s → %-10lld (期望 %lld)\n",
                   got == t.expect ? "OK " : "!! ", t.name, got, t.expect);
        }
    }

    cout << "\n===== [4] 修正版 vs 暴力 对拍（50000 组随机）=====\n";
    {
        mt19937_64 rng(20260917);
        int bad = 0, zeroAns = 0, oneAns = 0, total = 0;
        for (int tc = 0; tc < 50000; ++tc) {
            int n = 1 + rng() % 5;
            vector<ll> v(n);
            ll sum = 0;
            for (auto& x : v) { x = 1 + rng() % 30; sum += x; }
            ll k = 1 + rng() % (sum + 3);          // 有时故意让 k 超过总和
            ll e = brute(v, k);
            ++total;
            if (e == 0) zeroAns++;
            if (e == 1) oneAns++;
            if (solve_fixed(v, k) != e) bad++;
        }
        printf("  50000 组：修正版不一致 %d 组 → %s\n", bad, bad == 0 ? "正确 ✔" : "有问题 ✘");
        printf("  其中答案=0 的 %d 组、答案=1 的 %d 组（这两类正是原码出问题的区间）\n", zeroAns, oneAns);

        // 统计原码在"答案=1"这一档的崩溃率
        mt19937_64 r2(20260917);
        int crash = 0, one = 0;
        for (int tc = 0; tc < 20000; ++tc) {
            int n = 1 + r2() % 5;
            vector<ll> v(n);
            ll sum = 0;
            for (auto& x : v) { x = 1 + r2() % 30; sum += x; }
            ll k = 1 + r2() % (sum + 3);
            if (brute(v, k) != 1) continue;
            ++one;
            int zs = -1; vector<string> tr;
            find_traced(v, k, 0, 1000000000LL, zs, tr);
            if (zs > 0) ++crash;
        }
        printf("  原码在「答案=1」的 %d 组里，mid 取到 0（即崩溃）的：%d 组\n", one, crash);
    }
    return 0;
}
