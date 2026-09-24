// 对照实验：把"后序"换成"BFS 访问顺序"，改成倒着删最后 k 个，还能保证连通吗？
#include <bits/stdc++.h>
using namespace std;

int n, m, k;
vector<string> g;
int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m >> k;
    g.resize(n);
    for (int i = 0; i < n; ++i) cin >> g[i];

    int sx = -1, sy = -1;
    for (int i = 0; i < n && sx < 0; ++i)
        for (int j = 0; j < m; ++j)
            if (g[i][j] == '.') { sx = i; sy = j; break; }

    vector<vector<char>> vis(n, vector<char>(m, 0));
    vector<pair<int,int>> ord;
    queue<pair<int,int>> q;
    q.push({sx, sy}); vis[sx][sy] = 1;
    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        ord.push_back({x, y});
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
            if (g[nx][ny] != '.' || vis[nx][ny]) continue;
            vis[nx][ny] = 1; q.push({nx, ny});
        }
    }
    // 倒着删最后 k 个
    for (int i = 0; i < k && i < (int)ord.size(); ++i) {
        auto [x, y] = ord[ord.size() - 1 - i];
        g[x][y] = 'X';
    }
    for (int i = 0; i < n; ++i) cout << g[i] << "\n";
    return 0;
}
