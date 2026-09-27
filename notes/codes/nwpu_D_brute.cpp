// NWPUPC 2026 Problem D —— 独立暴力：从小到大枚举 R，第一个可行的就是最小 R
// 与"二分"是完全不同的推理（线性扫描 + 同一个必要性计算的独立写法）
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ll N, K;
    if (!(cin >> N >> K)) return 0;
    vector<ll> x(N);
    for (ll i = 0; i < N; ++i) cin >> x[i];

    ll span = x[N - 1] - x[0];
    for (ll R = 1; R <= max(1LL, span); ++R) {
        ll need = 0;
        for (ll i = 0; i + 1 < N; ++i) {
            ll d = x[i + 1] - x[i];
            ll cnt = 0;                            // 独立写法：直接循环数需要几个中继
            ll rest = d;
            while (rest > R) { rest -= R; ++cnt; }
            need += cnt;
        }
        if (need <= K) { cout << R * R << "\n"; return 0; }
    }
    cout << -1 << "\n";
    return 0;
}
