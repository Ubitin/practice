// 点灯照亮格子 —— 正解 O(H*W)
//
// 核心：把"每个格子都真的往四个方向走一遍"（O(H*W*max(H,W))，H,W=2000 时约 1.6e10，必 TLE）
//       改成"每格只看它【相邻的那一格】就能算出答案"（一遍递推）。
//
// 记 up[i][j] = 从 (i,j) 往上（含自己）连续空格的个数
//   若 g[i][j]=='#'  → up[i][j] = 0
//   否则 up[i][j] = 1 + (i>0 ? up[i-1][j] : 0)
//   ⇒ 这恰好就是"灯放在 (i,j) 时，这一列上方（含自己）能照亮的格子数"
//   同理 down / left / right 三个方向。
//
// 灯放在空格 (i,j) 时：
//   答案 = up + down + left + right - 3     ← (i,j) 自己被四个方向各算了一次，多算 3 次
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int H, W;
    if (!(cin >> H >> W)) return 0;
    vector<string> g(H);
    for (int i = 0; i < H; ++i) cin >> g[i];

    // 用 uint16_t 存：每个方向最多 2000 格，2 字节足够 ⇒ 4 张表共 4*4e6*2 = 32MB
    vector<vector<uint16_t>> up(H, vector<uint16_t>(W, 0)), dn(H, vector<uint16_t>(W, 0)),
                             lf(H, vector<uint16_t>(W, 0)), rt(H, vector<uint16_t>(W, 0));

    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
            if (g[i][j] == '.') up[i][j] = 1 + (i > 0 ? up[i - 1][j] : 0);
    for (int i = H - 1; i >= 0; --i)
        for (int j = 0; j < W; ++j)
            if (g[i][j] == '.') dn[i][j] = 1 + (i + 1 < H ? dn[i + 1][j] : 0);
    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
            if (g[i][j] == '.') lf[i][j] = 1 + (j > 0 ? lf[i][j - 1] : 0);
    for (int i = 0; i < H; ++i)
        for (int j = W - 1; j >= 0; --j)
            if (g[i][j] == '.') rt[i][j] = 1 + (j + 1 < W ? rt[i][j + 1] : 0);

    int best = 0;
    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
            if (g[i][j] == '.')
                best = max(best, (int)up[i][j] + dn[i][j] + lf[i][j] + rt[i][j] - 3);

    cout << best << "\n";
    return 0;
}
