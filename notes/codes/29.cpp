#include <bits/stdc++.h>
using namespace std;

int main() {
    // ========== 一维前缀和 ==========
    int n = 6;
    vector<int> a = {0, 1, 3, 2, 5, 4, 2};      // 1-based: a[1..6]
    vector<long long> pre(n + 1, 0);
    for (int i = 1; i <= n; i++) pre[i] = pre[i - 1] + a[i];
    // 区间和 [l, r] = pre[r] - pre[l-1]
    auto sum = [&](int l, int r) { return pre[r] - pre[l - 1]; };
    // 暴力对拍验证
    bool ok1 = true;
    for (int l = 1; l <= n; l++)
        for (int r = l; r <= n; r++) {
            long long bf = 0;
            for (int i = l; i <= r; i++) bf += a[i];
            if (bf != sum(l, r)) ok1 = false;
        }
    cout << "一维前缀和: 区间和与暴力全部一致 = " << ok1
         << "  [2,5]=" << sum(2, 5) << " (3+2+5+4=14)\n";

    // ========== 差分（前缀和的逆运算）：区间加 ==========
    vector<long long> diff(n + 2, 0);
    auto add = [&](int l, int r, int k) { diff[l] += k; diff[r + 1] -= k; };
    add(2, 5, 10);                              // [2,5] 每个 +10
    add(1, 3, 5);                               // [1,3] 每个 +5
    vector<long long> res(n + 1, 0);
    for (int i = 1; i <= n; i++) res[i] = res[i - 1] + diff[i];   // 差分前缀和还原
    cout << "差分区间加后: ";
    for (int i = 1; i <= n; i++) cout << res[i] << " ";
    cout << " (原 1 3 2 5 4 2 → 6 18 17 15 14 2)\n";

    // ========== 二维前缀和 ==========
    int R = 3, C = 4;
    int m2[4][5] = {{0}, {0, 1, 2, 3, 4}, {0, 5, 6, 7, 8}, {0, 9, 10, 11, 12}}; // 1-based
    long long p2[4][5] = {};
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            p2[i][j] = p2[i - 1][j] + p2[i][j - 1] - p2[i - 1][j - 1] + m2[i][j];
    auto sub = [&](int x1, int y1, int x2, int y2) {
        return p2[x2][y2] - p2[x1 - 1][y2] - p2[x2][y1 - 1] + p2[x1 - 1][y1 - 1];
    };
    bool ok2 = true;
    for (int x1 = 1; x1 <= R; x1++) for (int y1 = 1; y1 <= C; y1++)
        for (int x2 = x1; x2 <= R; x2++) for (int y2 = y1; y2 <= C; y2++) {
            long long bf = 0;
            for (int i = x1; i <= x2; i++) for (int j = y1; j <= y2; j++) bf += m2[i][j];
            if (bf != sub(x1, y1, x2, y2)) ok2 = false;
        }
    cout << "二维前缀和: 子矩阵和与暴力全部一致 = " << ok2
         << "  [1,1]-[2,2]=" << sub(1, 1, 2, 2) << " (1+2+5+6=14)\n";

    // ========== 二维差分：子矩阵加（P3397 地毯套路）==========
    int n2 = 4;
    long long d2[6][6] = {};
    auto rect = [&](int x1, int y1, int x2, int y2) {
        d2[x1][y1] += 1; d2[x2 + 1][y1] -= 1;
        d2[x1][y2 + 1] -= 1; d2[x2 + 1][y2 + 1] += 1;   // 四角标记
    };
    rect(1, 1, 2, 2); rect(2, 2, 3, 3);
    long long ans2[6][6] = {};
    for (int i = 1; i <= n2; i++)          // 两次前缀和还原
        for (int j = 1; j <= n2; j++)
            ans2[i][j] = d2[i][j] + ans2[i - 1][j] + ans2[i][j - 1] - ans2[i - 1][j - 1];
    cout << "二维差分(两块地毯覆盖数):\n";
    for (int i = 1; i <= n2; i++) { for (int j = 1; j <= n2; j++) cout << ans2[i][j] << " "; cout << "\n"; }
    return 0;
}
