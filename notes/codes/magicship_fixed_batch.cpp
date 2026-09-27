// CF1117C 正解（批量版，仅供对拍）：二分答案 + 风的前缀和，读到 EOF 为止
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll x1, y1, x2, y2, n;
    while (cin >> x1 >> y1 >> x2 >> y2 >> n) {
        string s; cin >> s;
        vector<ll> px(n + 1, 0), py(n + 1, 0);
        for (ll i = 1; i <= n; ++i) {
            ll dx = 0, dy = 0;
            if (s[i - 1] == 'U') dy = 1;
            else if (s[i - 1] == 'D') dy = -1;
            else if (s[i - 1] == 'L') dx = -1;
            else if (s[i - 1] == 'R') dx = 1;
            px[i] = px[i - 1] + dx;
            py[i] = py[i - 1] + dy;
        }
        ll fx = px[n], fy = py[n];
        auto ok = [&](ll d) {
            ll full = d / n, rem = d % n;
            ll wx = fx * full + px[rem], wy = fy * full + py[rem];
            return llabs(x2 - (x1 + wx)) + llabs(y2 - (y1 + wy)) <= d;
        };
        ll lo = 0, hi = 4000000000000000000LL / n * n;
        if (!ok(hi)) { cout << -1 << "\n"; continue; }
        while (lo < hi) { ll mid = lo + (hi - lo) / 2; if (ok(mid)) hi = mid; else lo = mid + 1; }
        cout << lo << "\n";
    }
    return 0;
}
