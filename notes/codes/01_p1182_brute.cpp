// ============================================================
//  01_p1182_brute.cpp —— P1182 暴力解（对拍的裁判）
//
//  数据规模必须小（n <= 60 左右）：DP 枚举所有分段方案
//    dp[i][j] = 把前 i 个数分成 j 段时的【最小「最大段和」】
//    dp[i][j] = min over k<i of max( dp[k][j-1], sum(k+1..i) )
//  答案 = dp[n][m]
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ll n, m;
    if (!(cin >> n >> m)) return 0;
    vector<ll> a(n + 1), pre(n + 1, 0);
    for (ll i = 1; i <= n; ++i) { cin >> a[i]; pre[i] = pre[i - 1] + a[i]; }

    const ll INF = LLONG_MAX / 4;
    vector<vector<ll>> dp(n + 1, vector<ll>(m + 1, INF));
    dp[0][0] = 0;
    for (ll i = 1; i <= n; ++i)
        for (ll j = 1; j <= m; ++j)
            for (ll k = 0; k < i; ++k)
                if (dp[k][j - 1] != INF)
                    dp[i][j] = min(dp[i][j], max(dp[k][j - 1], pre[i] - pre[k]));

    cout << dp[n][m] << '\n';
    return 0;
}
