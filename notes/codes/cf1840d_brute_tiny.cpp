// CF1840D —— 另一版暴力（只适用于值域很小）：枚举三个整数型号的所有组合
// 用途：交叉验证"切两刀"那版暴力在小数据上是否等价
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        int lo = *min_element(a.begin(), a.end());
        int hi = *max_element(a.begin(), a.end());
        int best = INT_MAX;
        for (int x = lo; x <= hi; ++x)
            for (int y = x; y <= hi; ++y)
                for (int z = y; z <= hi; ++z) {
                    int worst = 0;
                    for (int v : a) worst = max(worst, min(abs(v - x), min(abs(v - y), abs(v - z))));
                    best = min(best, worst);
                }
        cout << best << "\n";
    }
    return 0;
}
