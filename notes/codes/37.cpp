// ============================================================
//  test_/37.cpp —— 错排（derangement）公式实测
//
//  配套讲义：讲义/错排公式详解.md
//
//  错排：把 1..n 重新排列，使每个元素都不在自己的位置上（π(i) ≠ i）。
//        这样的排列个数记作 D_n（也叫 !n、derangement number）。
//
//  本文验证三条公式 + 近似式 + 溢出边界 + 竞赛用法。
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

static void title(const char* s) { printf("\n========== %s ==========\n", s); }

// ---------- ① 暴力：枚举所有排列数不动点 ----------
static ll bruteDerange(int n) {
    vector<int> p(n);
    iota(p.begin(), p.end(), 0);
    ll cnt = 0;
    do {
        bool ok = true;
        for (int i = 0; i < n; ++i) if (p[i] == i) { ok = false; break; }
        if (ok) ++cnt;
    } while (next_permutation(p.begin(), p.end()));
    return cnt;
}

// 暴力：恰好有 k 个不动点的排列数
static ll bruteFixed(int n, int k) {
    vector<int> p(n);
    iota(p.begin(), p.end(), 0);
    ll cnt = 0;
    do {
        int f = 0;
        for (int i = 0; i < n; ++i) if (p[i] == i) ++f;
        if (f == k) ++cnt;
    } while (next_permutation(p.begin(), p.end()));
    return cnt;
}

// ---------- ② 三条公式 ----------
// 递推一：D_n = (n-1)(D_{n-1} + D_{n-2})，D_0=1, D_1=0
static vector<ll> byRec1(int N) {
    vector<ll> D(N + 1);
    D[0] = 1; if (N >= 1) D[1] = 0;
    for (int n = 2; n <= N; ++n) D[n] = (ll)(n - 1) * (D[n - 1] + D[n - 2]);
    return D;
}
// 递推二：D_n = n·D_{n-1} + (-1)^n
static vector<ll> byRec2(int N) {
    vector<ll> D(N + 1);
    D[0] = 1;
    for (int n = 1; n <= N; ++n) D[n] = (ll)n * D[n - 1] + (n % 2 ? -1 : 1);
    return D;
}
// 容斥（整数版）：D_n = Σ_{k=0}^{n} (-1)^k · C(n,k) · (n-k)!
static vector<ll> byIE(int N) {
    vector<vector<ll>> C(N + 1, vector<ll>(N + 1, 0));
    for (int i = 0; i <= N; ++i) { C[i][0] = 1; for (int j = 1; j <= i; ++j) C[i][j] = C[i-1][j-1] + C[i-1][j]; }
    vector<ll> fact(N + 1, 1);
    for (int i = 1; i <= N; ++i) fact[i] = fact[i-1] * i;
    vector<ll> D(N + 1, 0);
    for (int n = 0; n <= N; ++n) {
        ll s = 0;
        for (int k = 0; k <= n; ++k) s += (k % 2 ? -1 : 1) * C[n][k] * fact[n - k];
        D[n] = s;
    }
    return D;
}

int main() {
    // ---------------------------------------------------------
    title("[1] 定义验证：暴力枚举所有排列（n <= 9）");
    {
        printf("  n :  暴力枚举  =  D_n\n");
        vector<ll> D = byRec1(9);
        bool allOk = true;
        for (int n = 1; n <= 9; ++n) {
            ll b = bruteDerange(n);
            printf("  %d : %10lld  =  %lld  %s\n", n, b, D[n], b == D[n] ? "✔" : "✘");
            if (b != D[n]) allOk = false;
        }
        printf("  → %s\n", allOk ? "暴力与公式完全一致 ✔" : "不一致 ✘");
    }

    // ---------------------------------------------------------
    title("[2] 三条公式互相对照（n = 0..20）");
    {
        const int N = 20;
        auto A = byRec1(N), B = byRec2(N), C = byIE(N);
        bool ok = true;
        for (int n = 0; n <= N; ++n) if (A[n] != B[n] || A[n] != C[n]) ok = false;
        printf("  递推一 (n-1)(D_{n-1}+D_{n-2}) 与 递推二 n·D_{n-1}+(-1)^n\n");
        printf("  与 容斥 Σ(-1)^k C(n,k)(n-k)!  三者：%s\n", ok ? "全部一致 ✔" : "有分歧 ✘");
        printf("  n=0..10 的 D_n：");
        for (int n = 0; n <= 10; ++n) printf("%lld ", A[n]);
        printf("\n");
    }

    // ---------------------------------------------------------
    title("[3] D_n 完整表（n = 0..25，注意溢出）");
    {
        auto D = byRec1(25);
        for (int n = 0; n <= 25; ++n) {
            bool of = false;
            // 检测：用 __int128 重算一遍比对，看 ll 版是否已溢出
            __int128 a = 1, b = 0;      // D_0, D_1
            if (n == 0) a = 1; else if (n == 1) a = 0;
            else { for (int i = 2; i <= n; ++i) { __int128 c = (__int128)(i - 1) * (a + b); b = a; a = c; } }
            of = (a > (__int128)LLONG_MAX);
            printf("  D_%-2d = %-22lld %s\n", n, D[n], of ? "← 【已溢出 long long】" : "");
        }
        // 精确找出溢出界
        __int128 a = 1, b = 0;
        int lastOk = 0;
        for (int n = 0; n <= 25; ++n) {
            if (n == 0) { a = 1; }
            else if (n == 1) { a = 0; b = 1; }
            else { __int128 c = (__int128)(n - 1) * (a + b); b = a; a = c; }
            if (a <= (__int128)LLONG_MAX && a >= 0) lastOk = n; else break;
        }
        printf("  → long long 能装下的最大下标：n = %d（D_%d = %lld）\n", lastOk, lastOk, byRec1(lastOk)[lastOk]);
        printf("  → LLONG_MAX = %lld\n", LLONG_MAX);
    }

    // ---------------------------------------------------------
    title("[4] 近似公式：D_n = round(n!/e) 从哪个 n 开始成立？");
    {
        auto D = byRec1(20);
        const ld E = expl(1.0L);
        printf("  n :  round(n!/e)        D_n               是否相等\n");
        int firstOk = -1;
        for (int n = 1; n <= 20; ++n) {
            ld f = 1; for (int i = 1; i <= n; ++i) f *= i;
            ll approx = (ll)llroundl(f / E);
            bool eq = (approx == D[n]);
            if (eq && firstOk < 0) firstOk = n;
            if (n <= 12 || !eq) printf("  %2d: %-19lld %-18lld %s\n", n, approx, D[n], eq ? "✔" : "✘");
        }
        printf("  → 从 n = %d 起完全成立（实测用 long double）\n", firstOk);
        printf("  → 理论上对所有 n >= 1 都成立；n 很大时 long double 精度会不够\n");
    }

    // ---------------------------------------------------------
    title("[5] 恰好 k 个不动点：C(n,k) · D_{n-k}（暴力验证）");
    {
        auto D = byRec1(9);
        printf("  暴力枚举 vs C(n,k)*D_{n-k}：\n");
        bool allOk = true;
        for (int n = 3; n <= 8; ++n) {
            printf("   n=%d : ", n);
            for (int k = 0; k <= n; ++k) {
                ll b = bruteFixed(n, k);
                ll Cnk = 1; for (int i = 1; i <= k; ++i) Cnk = Cnk * (n - i + 1) / i;
                ll f = Cnk * D[n - k];
                if (b != f) allOk = false;
                printf("k=%d:%lld%s ", k, b, b == f ? "" : "✘");
            }
            printf("\n");
        }
        printf("  → %s\n", allOk ? "全部一致 ✔（恰好 k 个不动点 = C(n,k)·D_{n-k}）" : "有分歧 ✘");
    }

    // ---------------------------------------------------------
    title("[6] 模意义下的算法（递推 vs 容斥逆元）对拍");
    {
        const ll MOD = 1000000007LL;
        auto pw = [](ll b, ll e, ll m) { ll r = 1; b %= m; while (e) { if (e & 1) r = r * b % m; b = b * b % m; e >>= 1; } return r; };

        // 法一：递推取模
        auto modRec = [&](int N) {
            vector<ll> D(N + 1); D[0] = 1; if (N >= 1) D[1] = 0;
            for (int n = 2; n <= N; ++n) D[n] = (ll)(n - 1) * ((D[n-1] + D[n-2]) % MOD) % MOD;
            return D;
        };
        // 法二：容斥 n! * Σ (-1)^k / k!（用阶乘与逆元）
        auto modIE = [&](int N) {
            vector<ll> fact(N + 1, 1);
            for (int i = 1; i <= N; ++i) fact[i] = fact[i-1] * i % MOD;
            vector<ll> invfact(N + 1);
            invfact[N] = pw(fact[N], MOD - 2, MOD);
            for (int i = N; i >= 1; --i) invfact[i-1] = invfact[i] * i % MOD;
            vector<ll> D(N + 1, 0);
            ll s = 0;
            for (int n = 0; n <= N; ++n) {
                s = (s + (n % 2 ? MOD - invfact[n] : invfact[n])) % MOD;
                D[n] = fact[n] * s % MOD;
            }
            return D;
        };

        const int N = 3000;
        auto A = modRec(N), B = modIE(N);
        bool ok = true;
        for (int n = 0; n <= N; ++n) if (A[n] != B[n]) { ok = false; printf("  ✘ n=%d 递推=%lld 容斥=%lld\n", n, A[n], B[n]); }
        printf("  n = 0..%d：递推取模与容斥逆元 %s\n", N, ok ? "完全一致 ✔" : "有分歧 ✘");
        printf("  MOD=%lld 下：D_10=%lld  D_100=%lld  D_1000=%lld  D_3000=%lld\n",
               MOD, A[10], A[100], A[1000], A[3000]);
        printf("  ⚠️ 注意 D_1000 在精确值里是个 2568 位的天文数字，模意义下照样能算\n");
    }

    // ---------------------------------------------------------
    title("[7] P1595 信封问题（洛谷）实测");
    {
        auto D = byRec1(20);
        printf("  题意：n 封信装 n 个信封，全部装错有几种装法 → 答案就是 D_n\n");
        printf("  样例 n=2 → %lld（题面答案 1）\n", D[2]);
        printf("  n=1 → %lld ; n=3 → %lld ; n=4 → %lld\n", D[1], D[3], D[4]);
        printf("  n=20 → %lld  ← 约束为什么恰好是 n<=20：D_21 就超 long long 了\n", D[20]);
    }

    printf("\n===== 实测结束 =====\n");
    return 0;
}
