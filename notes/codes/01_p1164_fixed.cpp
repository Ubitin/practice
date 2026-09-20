// P1164 小A点菜 —— 0/1 背包计数（方案数）
// 思路：dp[j] = 用已考虑过的菜、恰好凑出 j 元的方案数
//       dp[0] = 1（空集），每道菜倒序更新一次
#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m; cin >> n >> m;

    vector<long long> dp(m + 1, 0);
    dp[0] = 1;                                  // 凑 0 元：什么都不点，1 种方案

    for (int i = 0; i < n; ++i) {
        int x; cin >> x;
        for (int j = m; j >= x; --j)            // 倒序！否则同一道菜会被用多次
            dp[j] += dp[j - x];
    }

    cout << dp[m] << "\n";
    return 0;
}
