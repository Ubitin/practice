// P5019 铺设道路 —— 参考解（分治版，与"差分之谜"是两套完全不同的推理）
//
// 分治推理：
//   考虑区间 [l,r]。区间内最低的那一层的土，可以【一次操作铺满整个区间】直接填掉
//   （因为区间内每段都至少有 min 个单位要填），代价 min。
//   填完之后，"最低点"变成 0，整个区间就从 0 的位置断成若干段，
//   每一段各自独立求解 ⇒ 递归。
//   ans(l,r) = min(a[l..r]) + Σ ans(各段)
//
// 这与公式解 ans = a[0] + Σ max(0, a[i]-a[i-1]) 在推理上毫无关系，
// 用它来对拍就能验证公式解"不是碰巧"。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int n;
vector<ll> a;

ll divide(int l, int r) {          // 闭区间 [l, r]，保证 l <= r
    ll mn = LLONG_MAX;
    for (int i = l; i <= r; ++i) mn = min(mn, a[i]);
    ll res = mn;                   // 先一次把最低那层铺满
    for (int i = l; i <= r; ++i) a[i] -= mn;   // 填掉后必然出现 0
    int i = l;
    while (i <= r) {
        if (a[i] == 0) { ++i; continue; }
        int j = i;
        while (j + 1 <= r && a[j + 1] != 0) ++j;   // a[i..j] 是一段非零
        res += divide(i, j);
        i = j + 1;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (!(cin >> n)) return 0;
    a.assign(n, 0);
    for (int i = 0; i < n; ++i) cin >> a[i];
    if (n == 0) { cout << 0 << "\n"; return 0; }
    // 跳过全 0 的前缀/后缀，保证 divide 的入口是一段非零
    int L = 0; while (L < n && a[L] == 0) ++L;
    int R = n - 1; while (R >= 0 && a[R] == 0) --R;
    if (L > R) { cout << 0 << "\n"; return 0; }
    cout << divide(L, R) << "\n";
    return 0;
}
