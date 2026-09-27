// NWPUPC 2026 D —— 批量对拍用的两份实现（读到 EOF），逻辑与单份版一致
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#ifdef BRUTE
int main() {
    ll N, K;
    while (cin >> N >> K) {
        vector<ll> x(N);
        for (ll i = 0; i < N; ++i) cin >> x[i];
        ll span = x[N - 1] - x[0];
        ll ans = -1;
        for (ll R = 1; R <= max(1LL, span); ++R) {
            ll need = 0;
            for (ll i = 0; i + 1 < N; ++i) {
                ll rest = x[i + 1] - x[i], cnt = 0;
                while (rest > R) { rest -= R; ++cnt; }
                need += cnt;
            }
            if (need <= K) { ans = R * R; break; }
        }
        cout << ans << "\n";
    }
    return 0;
}
#else
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll N, K;
    while (cin >> N >> K) {
        vector<ll> x(N);
        for (ll i = 0; i < N; ++i) cin >> x[i];
        auto check = [&](ll R) {
            ll need = 0;
            for (ll i = 0; i + 1 < N; ++i) {
                ll d = x[i + 1] - x[i];
                if (d > R) { need += (d - 1) / R; if (need > K) return false; }
            }
            return need <= K;
        };
        ll lo = 1, hi = x[N - 1] - x[0];
        while (lo < hi) { ll mid = lo + (hi - lo) / 2; if (check(mid)) hi = mid; else lo = mid + 1; }
        cout << lo * lo << "\n";
    }
    return 0;
}
#endif
