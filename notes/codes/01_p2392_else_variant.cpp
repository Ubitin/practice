// ============================================================
//  01_p2392_else_variant.cpp —— 「不选」分支写成 else 的变体（故意写错的对照版）
//
//  与 test_/01.cpp 的唯一区别：
//      原版：if(装得下){ 选; dfs; 撤销; }
//            dfs(k+1);              ← 「不选」无条件执行（if 外面）
//      本版：if(装得下){ 选; dfs; 撤销; }
//            else { dfs(k+1); }     ← 「不选」只在装不下时才走
//
//  本文件用来说明：写成 else 就退化成「能装就装的贪心」，
//  会漏掉「当前这道题先不做、留给后面更合适的组合」这条路 → 漏最优解。
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll s[5], a[22];
ll ans = 0, maxdeep, sum;
ll maxtime, nowtime;

void dfs(ll k) {
    if (k > maxdeep) {
        maxtime = max(maxtime, nowtime);
        return;
    }
    if ((nowtime + a[k]) <= (sum / 2)) {
        nowtime += a[k];
        dfs(k + 1);
        nowtime -= a[k];
    } else {
        dfs(k + 1);          // ← 只有「装不下」才允许不选
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    memset(s, 0, sizeof(s)); memset(a, 0, sizeof(a));
    for (int i = 0; i < 4; ++i) cin >> s[i];

    for (ll i = 0; i < 4; ++i) {
        maxtime = 0; nowtime = 0;
        maxdeep = s[i]; sum = 0;
        for (ll j = 1; j <= s[i]; ++j) { cin >> a[j]; sum += a[j]; }
        dfs(1);
        ans += (sum - maxtime);
    }
    cout << ans << '\n';
    return 0;
}
