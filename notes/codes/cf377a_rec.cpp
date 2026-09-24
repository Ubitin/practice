// CF377A 的【递归版】参考解 —— 只用来说明"递归在 500×500 蛇形数据上会爆栈"
// 不要照抄这个版本提交（除非本地编译加 -Wl,--stack,268435456）
#include <bits/stdc++.h>
using namespace std;
int n, m, k;
vector<string> g;
vector<pair<int,int>> ord;
int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};

void dfs(int x, int y) {
    g[x][y] = 'V';                              // 临时标记已访问
    for (int d = 0; d < 4; ++d) {
        int nx = x + dx[d], ny = y + dy[d];
        if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
        if (g[nx][ny] != '.') continue;
        dfs(nx, ny);
    }
    ord.push_back({x, y});                      // 后序
}
int main() {
    cin >> n >> m >> k;
    g.resize(n);
    for (int i = 0; i < n; ++i) cin >> g[i];
    int sx = -1, sy = -1;
    for (int i = 0; i < n && sx < 0; ++i)
        for (int j = 0; j < m; ++j) if (g[i][j] == '.') { sx = i; sy = j; break; }
    dfs(sx, sy);
    for (auto& r : g) for (auto& c : r) if (c == 'V') c = '.';
    for (int i = 0; i < k && i < (int)ord.size(); ++i) g[ord[i].first][ord[i].second] = 'X';
    for (int i = 0; i < n; ++i) cout << g[i] << "\n";
    return 0;
}
