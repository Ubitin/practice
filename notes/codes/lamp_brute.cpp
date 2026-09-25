// 点灯照亮格子 —— 暴力解（O(H*W*max(H,W))，只用来验证/对照思路，H,W 大时必 TLE）
#include <bits/stdc++.h>
using namespace std;

int H, W;
vector<string> g;

// 从 (x,y) 放灯，返回被照亮的格子数（灯自己也算，障碍物不算、也挡光）
int light(int x, int y) {
    if (g[x][y] == '#') return -1;                // 不能放在障碍物上
    vector<vector<char>> lit(H, vector<char>(W, 0));
    lit[x][y] = 1;
    const int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
    for (int d = 0; d < 4; ++d) {
        int cx = x + dx[d], cy = y + dy[d];
        while (cx >= 0 && cx < H && cy >= 0 && cy < W && g[cx][cy] == '.') {
            lit[cx][cy] = 1;
            cx += dx[d]; cy += dy[d];
        }
    }
    int cnt = 0;
    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
            if (lit[i][j]) ++cnt;
    return cnt;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (!(cin >> H >> W)) return 0;
    g.resize(H);
    for (int i = 0; i < H; ++i) cin >> g[i];

    int best = 0;
    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
            if (g[i][j] == '.') best = max(best, light(i, j));
    cout << best << "\n";
    return 0;
}
