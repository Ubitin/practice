// CF1117C 暴力解（批量版，仅供对拍）：读到 EOF 为止
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ll x1, y1, x2, y2, n;
    while (cin >> x1 >> y1 >> x2 >> y2 >> n) {
        string s; cin >> s;
        if (x1 == x2 && y1 == y2) { cout << 0 << "\n"; continue; }   // ★ 0 天就在终点
        set<pair<ll,ll>> cur{{x1, y1}};
        const ll LIM = 300;
        ll ans = -1;
        for (ll day = 1; day <= LIM; ++day) {
            set<pair<ll,ll>> nxt;
            char w = s[(day - 1) % n];
            ll wdx = 0, wdy = 0;
            if (w == 'U') wdy = 1;
            else if (w == 'D') wdy = -1;
            else if (w == 'L') wdx = -1;
            else if (w == 'R') wdx = 1;
            for (auto& p : cur) {
                ll bx = p.first + wdx, by = p.second + wdy;
                for (int d = 0; d < 5; ++d) {
                    ll cx = bx + (d == 1 ? -1 : d == 2 ? 1 : 0);
                    ll cy = by + (d == 3 ? -1 : d == 4 ? 1 : 0);
                    if (llabs(cx) > 1000 || llabs(cy) > 1000) continue;
                    nxt.insert({cx, cy});
                }
            }
            if (nxt.count({x2, y2})) { ans = day; break; }
            cur = nxt;
        }
        cout << ans << "\n";
    }
    return 0;
}
