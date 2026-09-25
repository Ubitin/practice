// 四种"删前 k 个"策略的对照自检：A=DFS 后序前缀  B=BFS 层序后缀  C=DFS 先序前缀  D=BFS 层序前缀
// 每组随机迷宫都跑 4 种策略并判定合法性，报出各自失败组数与最小反例
#include <bits/stdc++.h>
using namespace std;
using P = pair<int,int>;
int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};

struct Maze { int n, m, k; vector<string> g; };

Maze gen(long long seed, int maxN = 9, int maxM = 9) {
    mt19937_64 rng((unsigned long long)seed * 1000003ULL + 31ULL);
    auto rnd = [&](int hi) { return hi <= 0 ? 0 : (int)(rng() % (unsigned long long)(hi + 1)); };
    int n = 1 + rnd(maxN - 1), m = 1 + rnd(maxM - 1);
    vector<string> g(n, string(m, '#'));
    int x = rnd(n - 1), y = rnd(m - 1);
    g[x][y] = '.';
    vector<P> cells{{x, y}};
    int want = 1 + rnd(n * m - 1), guard = 0;
    while ((int)cells.size() < want && guard++ < n * m * 20) {
        auto [cx, cy] = cells[rnd((int)cells.size() - 1)];
        int d = rnd(3), nx = cx + dx[d], ny = cy + dy[d];
        if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
        if (g[nx][ny] == '.') continue;
        g[nx][ny] = '.'; cells.push_back({nx, ny});
    }
    return {n, m, rnd(max(0, (int)cells.size() - 1)), g};
}

P startOf(const Maze& M) {
    for (int i = 0; i < M.n; ++i) for (int j = 0; j < M.m; ++j) if (M.g[i][j] == '.') return {i, j};
    return {-1, -1};
}

vector<P> orderOf(const Maze& M, int kind) {   // kind: 0=先序 1=后序 2=层序
    auto [sx, sy] = startOf(M);
    vector<vector<char>> vis(M.n, vector<char>(M.m, 0));
    vector<P> ord;
    if (kind == 2) {
        queue<P> q; q.push({sx, sy}); vis[sx][sy] = 1;
        while (!q.empty()) {
            auto [x, y] = q.front(); q.pop(); ord.push_back({x, y});
            for (int d = 0; d < 4; ++d) {
                int nx = x + dx[d], ny = y + dy[d];
                if (nx < 0 || nx >= M.n || ny < 0 || ny >= M.m) continue;
                if (M.g[nx][ny] != '.' || vis[nx][ny]) continue;
                vis[nx][ny] = 1; q.push({nx, ny});
            }
        }
        return ord;
    }
    // 迭代 DFS，用 (x,y,方向下标) 栈：进栈时(先序)或出栈时(后序)记录
    vector<array<int,3>> st{{sx, sy, 0}};
    vis[sx][sy] = 1;
    if (kind == 0) ord.push_back({sx, sy});
    while (!st.empty()) {
        auto& fr = st.back();
        if (fr[2] < 4) {
            int d = fr[2]++, nx = fr[0] + dx[d], ny = fr[1] + dy[d];
            if (nx < 0 || nx >= M.n || ny < 0 || ny >= M.m) continue;
            if (M.g[nx][ny] != '.' || vis[nx][ny]) continue;
            vis[nx][ny] = 1;
            if (kind == 0) ord.push_back({nx, ny});
            st.push_back({nx, ny, 0});
        } else {
            if (kind == 1) ord.push_back({fr[0], fr[1]});
            st.pop_back();
        }
    }
    return ord;
}

vector<string> del(const Maze& M, vector<P> o, bool fromBack) {
    auto g = M.g;
    for (int i = 0; i < M.k && i < (int)o.size(); ++i) {
        auto [x, y] = fromBack ? o[o.size() - 1 - i] : o[i];
        g[x][y] = 'X';
    }
    return g;
}

bool valid(const Maze& M, const vector<string>& B, string& why) {
    int X = 0, E0 = 0, E1 = 0;
    for (int i = 0; i < M.n; ++i) {
        if ((int)B[i].size() != M.m) { why = "行长度错"; return false; }
        for (int j = 0; j < M.m; ++j) {
            char a = M.g[i][j], b = B[i][j];
            E0 += (a == '.'); E1 += (b == '.');
            if (a == '#' && b != '#') { why = "# 被改"; return false; }
            if (a == '.' && b == 'X') ++X;
        }
    }
    if (X != M.k) { why = "变墙数不对"; return false; }
    if (E1 != E0 - M.k) { why = "剩余空格数不对"; return false; }
    if (E1 > 0) {
        vector<vector<char>> seen(M.n, vector<char>(M.m, 0));
        queue<P> q; int cnt = 0; bool first = true;
        for (int i = 0; i < M.n && first; ++i)
            for (int j = 0; j < M.m; ++j)
                if (B[i][j] == '.') { q.push({i, j}); seen[i][j] = 1; cnt = 1; first = false; break; }
        while (!q.empty()) {
            auto [x, y] = q.front(); q.pop();
            for (int d = 0; d < 4; ++d) {
                int nx = x + dx[d], ny = y + dy[d];
                if (nx < 0 || nx >= M.n || ny < 0 || ny >= M.m) continue;
                if (B[nx][ny] == '.' && !seen[nx][ny]) { seen[nx][ny] = 1; ++cnt; q.push({nx, ny}); }
            }
        }
        if (cnt != E1) { why = "剩余空格不连通"; return false; }
    }
    return true;
}

int main(int argc, char** argv) {
    int N = argc > 1 ? atoi(argv[1]) : 20000;
    const char* name[4] = {"A DFS后序删前缀", "B BFS层序删后缀", "C DFS先序删前缀", "D BFS层序删前缀"};
    int bad[4] = {0, 0, 0, 0};
    string shown[4];
    for (int s = 1; s <= N; ++s) {
        Maze M = gen(s);
        vector<P> pre = orderOf(M, 0), post = orderOf(M, 1), lay = orderOf(M, 2);
        struct { vector<P> o; bool back; } cand[4] = {{post, false}, {lay, true}, {pre, false}, {lay, false}};
        for (int c = 0; c < 4; ++c) {
            string why;
            if (!valid(M, del(M, cand[c].o, cand[c].back), why)) {
                if (++bad[c] <= 2) printf("  %s seed=%d: %s\n", name[c], s, why.c_str());
                if (shown[c].empty()) {
                    char buf[64]; snprintf(buf, sizeof buf, "%d %d %d\n", M.n, M.m, M.k);
                    shown[c] = buf;
                    for (auto& r : M.g) shown[c] += r + "\n";
                }
            }
        }
    }
    printf("\n随机 %d 组迷宫：\n", N);
    for (int c = 0; c < 4; ++c) printf("  %-16s 失败 %d 组\n", name[c], bad[c]);
    for (int c = 2; c < 4; ++c)
        if (!shown[c].empty()) printf("\n%s 的最小反例：\n%s", name[c], shown[c].c_str());
    return 0;
}
