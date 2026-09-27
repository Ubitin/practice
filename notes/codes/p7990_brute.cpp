// 国家补助（全局取 max 型题）—— 暴力解：严格按题意模拟，用来当标准答案
// 事件1: 1 p x  → a[p] = x
// 事件2: 2 x    → 对所有 a[i] < x 的公民，a[i] = x
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n;
    while (cin >> n) {                    // 批量对拍：读到 EOF 为止，一次喂多个用例
        vector<ll> a(n + 1);
        for (ll i = 1; i <= n; ++i) cin >> a[i];
        ll q; cin >> q;
        while (q--) {
            ll k; cin >> k;
            if (k == 1) { ll p, x; cin >> p >> x; a[p] = x; }
            else { ll x; cin >> x; for (ll i = 1; i <= n; ++i) if (a[i] < x) a[i] = x; }
        }
        for (ll i = 1; i <= n; ++i) cout << a[i] << " \n"[i == n];
    }
    return 0;
}
