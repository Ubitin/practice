#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while (t--) {
        int n, k; cin >> n >> k;
        const int INF = 1e9;
        vector<int> first(k + 1, INF), last(k + 1, -1);  // 每个值在 a 中首次/末次出现的下标

        for (int i = 0; i < n; ++i) {
            int x; cin >> x;
            first[x] = min(first[x], i);
            last[x]  = max(last[x],  i);
        }

        vector<int> ans(k + 1, 0);
        int L = INF, R = -1;                 // 当前 [c..k] 中存在值的下标范围
        for (int c = k; c >= 1; --c) {
            if (last[c] != -1) {             // 颜色 c 出现在 a 中
                L = min(L, first[c]);
                R = max(R, last[c]);
                ans[c] = 2 * (R - L + 1);    // 行跨度=列跨度（对称），矩形宽+高
            }
            // 颜色 c 不存在 → 表格里也没有颜色 c 的格子 → 保持 0
        }
        for (int c = 1; c <= k; ++c) cout << ans[c] << " \n"[c == k];
    }
    return 0;
}
