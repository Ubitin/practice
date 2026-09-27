// CF1117C Magic Ship —— 独立暴力解 v2（逐天扩散）
// 坐标映射与题目/用户代码一致：U → 第二维 +1、D → 第二维 −1、L → 第一维 −1、R → 第一维 +1
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ll x1, y1, x2, y2, n;
    cin >> x1 >> y1 >> x2 >> y2 >> n;
    string s; cin >> s;

    set<pair<ll,ll>> cur{{x1, y1}};
    const ll LIM = 300;
    for (ll day = 1; day <= LIM; ++day) {
        set<pair<ll,ll>> nxt;
        char w = s[(day - 1) % n];
        ll wdx = 0, wdy = 0;
        if (w == 'U') wdy = 1;
        else if (w == 'D') wdy = -1;
        else if (w == 'L') wdx = -1;
        else if (w == 'R') wdx = 1;
        for (auto& p : cur) {
            ll bx = p.first + wdx, by = p.second + wdy;      // ① 风
            for (int d = 0; d < 5; ++d) {                     // ② 船自己走（含不动）
                ll cx = bx + (d == 1 ? -1 : d == 2 ? 1 : 0);
                ll cy = by + (d == 3 ? -1 : d == 4 ? 1 : 0);
                if (llabs(cx) > 1000 || llabs(cy) > 1000) continue;
                nxt.insert({cx, cy});
            }
        }
        if (nxt.count({x2, y2})) { cout << day << "\n"; return 0; }
        cur = nxt;
    }
    cout << -1 << "\n";
    return 0;
}
