// P5019 交叉验证：分治参考解 vs 用户的公式解（两者推理完全不同）
// 生成器内嵌，风格轮换，直接对拍 + 报告
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

static ll formula(const vector<ll>& a) {
    if (a.empty()) return 0;
    ll ans = a[0];
    for (size_t i = 1; i < a.size(); ++i)
        if (a[i] > a[i - 1]) ans += a[i] - a[i - 1];
    return ans;
}

// 分治解（与 p5019_ref_divide.cpp 同一算法，内联以便对拍）
static ll divide(vector<ll>& a, int l, int r) {
    ll mn = LLONG_MAX;
    for (int i = l; i <= r; ++i) mn = min(mn, a[i]);
    ll res = mn;
    for (int i = l; i <= r; ++i) a[i] -= mn;
    int i = l;
    while (i <= r) {
        if (a[i] == 0) { ++i; continue; }
        int j = i;
        while (j + 1 <= r && a[j + 1] != 0) ++j;
        res += divide(a, i, j);
        i = j + 1;
    }
    return res;
}

static ll divideSolve(vector<ll> a) {
    int n = (int)a.size();
    int L = 0; while (L < n && a[L] == 0) ++L;
    int R = n - 1; while (R >= 0 && a[R] == 0) --R;
    if (L > R) return 0;
    return divide(a, L, R);
}

int main(int argc, char** argv) {
    unsigned seed = (argc > 1) ? (unsigned)atoi(argv[1]) : 1u;
    int rounds = (argc > 2) ? atoi(argv[2]) : 2000;
    mt19937 rng(seed);
    int mism = 0;
    for (int t = 0; t < rounds; ++t) {
        int style = t % 5;
        int n = 1 + (int)(rng() % 200);
        int hi;
        switch (style) {
            case 0: hi = 3;  break;                 // 值域极小 → 大量平局/零
            case 1: hi = 10; break;                 // 小值域
            case 2: hi = 100000; break;             // 全值域
            case 3: hi = 1000; break;               // 中等
            default: hi = 3; break;
        }
        vector<ll> a(n);
        for (int i = 0; i < n; ++i) {
            switch (style) {
                case 0: a[i] = rng() % (hi + 1); break;
                case 1: a[i] = 1 + rng() % hi; break;
                case 2: a[i] = rng() % (hi + 1); break;
                case 3: a[i] = (i % 2 == 0) ? (ll)(rng() % hi) : 0; break;  // 一半是 0
                default: a[i] = (rng() % 2) ? 0 : (ll)(rng() % hi); break;  // 稀疏
            }
        }
        if (style == 4) {   // 严格递增
            ll cur = 0;
            for (int i = 0; i < n; ++i) { cur += rng() % 5; a[i] = cur; }
        }
        ll f = formula(a), d = divideSolve(a);
        if (f != d) {
            ++mism;
            if (mism <= 3) {
                printf("不一致! n=%d 公式=%lld 分治=%lld\n  数组:", n, f, d);
                for (int i = 0; i < n && i < 30; ++i) printf(" %lld", a[i]);
                printf("\n");
            }
        }
    }
    printf("对拍 %d 轮（5 种风格轮换）：不一致 %d 个 %s\n", rounds, mism, mism ? "✘" : "✅");
    return mism ? 7 : 0;
}
