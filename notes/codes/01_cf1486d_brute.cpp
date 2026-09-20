// ============================================================
//  01_cf1486d_brute.cpp —— CF1486D 暴力解（对拍的"裁判"）
//
//  数据规模必须小（n <= 15 左右）才跑得动：
//  枚举所有长度 >= k 的子段，各自排序求中位数，取最大值。
//  中位数定义：长度为 m 的子段，中位数 = 升序后第 ⌈m/2⌉ 小。
// ============================================================
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    int best = INT_MIN;
    for (int l = 0; l < n; ++l) {
        vector<int> seg;
        for (int r = l; r < n; ++r) {
            seg.push_back(a[r]);
            int m = r - l + 1;
            if (m < k) continue;
            vector<int> s = seg;
            sort(s.begin(), s.end());
            best = max(best, s[(m + 1) / 2 - 1]);   // 第 ⌈m/2⌉ 小，转 0-based
        }
    }
    cout << best << '\n';
    return 0;
}
