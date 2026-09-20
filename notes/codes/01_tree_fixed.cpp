#include<bits/stdc++.h>
using namespace std;

int main() {
    int l, m;
    cin >> l >> m;
    vector<pair<int, int>> seg(m);
    for (int i = 0; i < m; ++i) cin >> seg[i].first >> seg[i].second;

    if (m == 0) { cout << l + 1 << "\n"; return 0; }   // 没有区间 → 一棵不移

    sort(seg.begin(), seg.end());          // pair 自动按 start 升序

    // 区间合并：统计实际被移除的树数
    int removed = 0;
    int curL = seg[0].first, curR = seg[0].second;
    for (int i = 1; i < m; ++i) {
        if (seg[i].first <= curR)          // 与当前合并区间重叠 → 扩展右端
            curR = max(curR, seg[i].second);
        else {                             // 断开 → 结算上一个合并区间
            removed += curR - curL + 1;
            curL = seg[i].first;
            curR = seg[i].second;
        }
    }
    removed += curR - curL + 1;            // 最后一个区间

    cout << (l + 1) - removed << "\n";     // 树有 L+1 棵（位置 0 ~ L）
    return 0;
}
