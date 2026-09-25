// 同一张迷宫，打印三种"名单"，并各自执行"删前 k 个"写到三个文件里
//   先序名单：进格子就记        后序名单：退回来才记        层序名单：BFS 一圈圈记
#include <bits/stdc++.h>
using namespace std;
using P = pair<int,int>;
int n, m, k;
vector<string> g;
int dx[4] = {1,-1,0,0}, dy[4] = {0,0,1,-1};

void dfsPre(int x, int y, vector<vector<char>>& vis, vector<P>& ord) {
    vis[x][y] = 1;
    ord.push_back({x, y});                      // ★ 进格子就记 = 先序
    for (int d = 0; d < 4; ++d) {
        int nx = x + dx[d], ny = y + dy[d];
        if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
        if (g[nx][ny] != '.' || vis[nx][ny]) continue;
        dfsPre(nx, ny, vis, ord);
    }
}

void dfsPost(int x, int y, vector<vector<char>>& vis, vector<P>& ord) {
    vis[x][y] = 1;
    for (int d = 0; d < 4; ++d) {
        int nx = x + dx[d], ny = y + dy[d];
        if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
        if (g[nx][ny] != '.' || vis[nx][ny]) continue;
        dfsPost(nx, ny, vis, ord);
    }
    ord.push_back({x, y});                      // ★ 四个方向看完才记 = 后序
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    cin >> n >> m >> k;
    g.resize(n);
    for (int i = 0; i < n; ++i) cin >> g[i];
    int sx = -1, sy = -1;
    for (int i = 0; i < n && sx < 0; ++i)
        for (int j = 0; j < m; ++j) if (g[i][j] == '.') { sx = i; sy = j; break; }

    vector<vector<char>> v1(n, vector<char>(m, 0)), v2(n, vector<char>(m, 0)), v3(n, vector<char>(m, 0));
    vector<P> pre, post, layer;
    dfsPre(sx, sy, v1, pre);
    dfsPost(sx, sy, v2, post);
    queue<P> q; q.push({sx, sy}); v3[sx][sy] = 1;
    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop(); layer.push_back({x, y});
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
            if (g[nx][ny] != '.' || v3[nx][ny]) continue;
            v3[nx][ny] = 1; q.push({nx, ny});
        }
    }

    auto show = [&](const char* name, vector<P>& o) {
        fprintf(stderr, "%-10s（前 6 个）: ", name);
        for (int i = 0; i < (int)o.size() && i < 6; ++i) fprintf(stderr, "(%d,%d) ", o[i].first, o[i].second);
        fprintf(stderr, "… 共 %zu 个\n", o.size());
    };
    show("先序名单", pre);
    show("后序名单", post);
    show("层序名单", layer);

    auto writeOut = [&](const char* file, vector<P>& o) {
        auto h = g;
        for (int i = 0; i < k && i < (int)o.size(); ++i) h[o[i].first][o[i].second] = 'X';
        FILE* f = fopen(file, "w");
        for (auto& r : h) fprintf(f, "%s\n", r.c_str());
        fclose(f);
    };
    writeOut("pre.out", pre);
    writeOut("post.out", post);
    writeOut("layer.out", layer);
    fprintf(stderr, "已写出 pre.out / post.out / layer.out（各自都是「删名单前 k 个」的结果）\n");
    return 0;
}
