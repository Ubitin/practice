// 点灯题：三种写法的实测耗时对比（2000×2000 极限数据）
//   ① 朴素：每个空格真的往四个方向走一遍        O(H*W*max(H,W))
//   ② 递推：每格只看相邻格                       O(H*W)   ← 推荐
//   ③ 递推 + int 版本（占内存更大，看是否影响速度）
#include <bits/stdc++.h>
using namespace std;

int H, W;
vector<string> g;

// ---------- ① 朴素写法 ----------
int naive() {
    int best = 0;
    const int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j) {
            if (g[i][j] == '#') continue;
            int cnt = 1;
            for (int d = 0; d < 4; ++d) {
                int cx = i + dx[d], cy = j + dy[d];
                while (cx >= 0 && cx < H && cy >= 0 && cy < W && g[cx][cy] == '.') {
                    ++cnt;
                    cx += dx[d]; cy += dy[d];
                }
            }
            best = max(best, cnt);
        }
    return best;
}

// ---------- ② 递推写法（uint16_t）----------
int fast16() {
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
    return best;
}

int main(int argc, char** argv) {
    int mode = (argc > 1) ? atoi(argv[1]) : 0;
    if (!(cin >> H >> W)) return 0;
    g.resize(H);
    for (int i = 0; i < H; ++i) cin >> g[i];

    auto t0 = chrono::steady_clock::now();
    int ans = 0;
    if (mode == 1) ans = naive();
    else           ans = fast16();
    auto t1 = chrono::steady_clock::now();
    long long ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
    printf("mode=%s  答案=%d  用时=%lld ms\n", mode == 1 ? "朴素(O(H*W*max))" : "递推(O(H*W))", ans, ms);
    return 0;
}
