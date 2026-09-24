// CF377A Maze —— 参考解（标准做法：DFS 后序，把"最先走完"的 k 个格子变成墙）
// 用【显式栈】的迭代 DFS，避免 500×500 的全空网格把递归栈撑爆（深度可达 25 万）
// 用法: cf377a_ref.exe [--trace]  < in.txt
#include <bits/stdc++.h>
using namespace std;

int n, m, k;
vector<string> g;
vector<pair<int,int>> order;          // DFS 后序（谁先走完谁先进这个表）
int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};

bool inside(int x, int y) { return x >= 0 && x < n && y >= 0 && y < m; }

int main(int argc, char** argv) {
    bool trace = (argc > 1 && string(argv[1]) == "--trace");
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> k;
    g.resize(n);
    for (int i = 0; i < n; ++i) cin >> g[i];

    // 找一个空格当搜索起点
    int sx = -1, sy = -1;
    for (int i = 0; i < n && sx < 0; ++i)
        for (int j = 0; j < m; ++j)
            if (g[i][j] == '.') { sx = i; sy = j; break; }

    vector<vector<char>> used(n, vector<char>(m, 0));
    vector<array<int,3>> st;                   // (x, y, 下一个要看的邻居下标)
    used[sx][sy] = 1;
    st.push_back({sx, sy, 0});
    while (!st.empty()) {
        auto& fr = st.back();
        int x = fr[0], y = fr[1];
        if (fr[2] < 4) {
            int d = fr[2]++;
            int nx = x + dx[d], ny = y + dy[d];
            if (inside(nx, ny) && g[nx][ny] == '.' && !used[nx][ny]) {
                used[nx][ny] = 1;
                st.push_back({nx, ny, 0});
            }
        } else {                               // 四个邻居都看完了 → 后序记录 + 出栈
            order.push_back({x, y});
            st.pop_back();
        }
    }

    if (trace) {
        fprintf(stderr, "空格总数 = %zu, k = %d\n后序（最先走完的在最前）:\n", order.size(), k);
        for (size_t i = 0; i < order.size(); ++i)
            fprintf(stderr, "  [%2zu] (%d,%d)%s\n", i, order[i].first, order[i].second,
                    i < (size_t)k ? "   ← 变成墙" : "");
    }

    for (int i = 0; i < k && i < (int)order.size(); ++i)
        g[order[i].first][order[i].second] = 'X';

    for (int i = 0; i < n; ++i) cout << g[i] << "\n";
    return 0;
}
