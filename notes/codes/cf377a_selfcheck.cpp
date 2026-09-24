// CF377A 自检：同一次运行里做「造数据 → 两种删法 → 判定合法性」，跑几万组
//   删法 A（标准）：DFS 后序，删【最先走完】的前 k 个
//   删法 B（常见错法）：BFS 访问顺序，倒着删最后 k 个
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
    int want = 1 + rnd(n * m - 1);
    int guard = 0;
    while ((int)cells.size() < want && guard++ < n * m * 20) {   // 从已有空格往外长 ⇒ 保证连通
        auto [cx, cy] = cells[rnd((int)cells.size() - 1)];
        int d = rnd(3), nx = cx + dx[d], ny = cy + dy[d];
        if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
        if (g[nx][ny] == '.') continue;
        g[nx][ny] = '.'; cells.push_back({nx, ny});
    }
    int E = (int)cells.size();
    int k = rnd(max(0, E - 1));
    return {n, m, k, g};
}

// 删法 A：迭代 DFS 后序（和参考解一致）
vector<string> removeA(Maze M) {
    auto g = M.g;
    int sx = -1, sy = -1;
    for (int i = 0; i < M.n && sx < 0; ++i)
        for (int j = 0; j < M.m; ++j) if (g[i][j] == '.') { sx = i; sy = j; break; }
    vector<vector<char>> used(M.n, vector<char>(M.m, 0));
    vector<array<int,3>> st{{sx, sy, 0}};
    used[sx][sy] = 1;
    vector<P> ord;
    while (!st.empty()) {
        auto& fr = st.back();
        if (fr[2] < 4) {
            int d = fr[2]++, nx = fr[0] + dx[d], ny = fr[1] + dy[d];
            if (nx >= 0 && nx < M.n && ny >= 0 && ny < M.m && g[nx][ny] == '.' && !used[nx][ny]) {
                used[nx][ny] = 1; st.push_back({nx, ny, 0});
            }
        } else { ord.push_back({fr[0], fr[1]}); st.pop_back(); }
    }
    for (int i = 0; i < M.k && i < (int)ord.size(); ++i) g[ord[i].first][ord[i].second] = 'X';
    return g;
}

// 删法 B：BFS 顺序倒着删
vector<string> removeB(Maze M) {
    auto g = M.g;
    int sx = -1, sy = -1;
    for (int i = 0; i < M.n && sx < 0; ++i)
        for (int j = 0; j < M.m; ++j) if (g[i][j] == '.') { sx = i; sy = j; break; }
    vector<vector<char>> vis(M.n, vector<char>(M.m, 0));
    vector<P> ord; queue<P> q;
    q.push({sx, sy}); vis[sx][sy] = 1;
    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop(); ord.push_back({x, y});
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            if (nx < 0 || nx >= M.n || ny < 0 || ny >= M.m) continue;
            if (g[nx][ny] != '.' || vis[nx][ny]) continue;
            vis[nx][ny] = 1; q.push({nx, ny});
        }
    }
    for (int i = 0; i < M.k && i < (int)ord.size(); ++i) {
        auto [x, y] = ord[ord.size() - 1 - i];
        g[x][y] = 'X';
    }
    return g;
}

// 判定：恰好 k 个新墙、`#` 没被改、剩下的 `.` 连通
bool valid(const Maze& M, const vector<string>& B, string& why) {
    int X = 0, E0 = 0, E1 = 0;
    for (int i = 0; i < M.n; ++i) {
        if ((int)B[i].size() != M.m) { why = "行长度错"; return false; }
        for (int j = 0; j < M.m; ++j) {
            char a = M.g[i][j], b = B[i][j];
            E0 += (a == '.');
            if (a == '#') { if (b != '#') { why = "# 被改了"; return false; } }
            else if (a == '.') {
                if (b == 'X') ++X;
                else if (b != '.') { why = "非法字符"; return false; }
            }
            E1 += (b == '.');
        }
    }
    if (X != M.k) { why = "变墙个数 " + to_string(X) + " ≠ k " + to_string(M.k); return false; }
    if (E1 != E0 - M.k) { why = "剩余空格数不对"; return false; }
    if (E1 > 0) {
        vector<vector<char>> seen(M.n, vector<char>(M.m, 0));
        int cnt = 0; queue<P> q; bool first = true;
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
        if (cnt != E1) { why = "剩余空格不连通（" + to_string(cnt) + "/" + to_string(E1) + "）"; return false; }
    }
    return true;
}

int main(int argc, char** argv) {
    int N = argc > 1 ? atoi(argv[1]) : 20000;
    int badA = 0, badB = 0;
    string firstB;
    for (int s = 1; s <= N; ++s) {
        Maze M = gen(s);
        string why;
        if (!valid(M, removeA(M), why)) { if (++badA <= 3) printf("  A 失败 seed=%d: %s\n", s, why.c_str()); }
        string why2;
        if (!valid(M, removeB(M), why2)) {
            if (++badB <= 3) {
                printf("  B 失败 seed=%d: %s\n", s, why2.c_str());
                if (firstB.empty()) {
                    string t;
                    char buf[128];
                    snprintf(buf, sizeof buf, "%d %d %d\n", M.n, M.m, M.k); t += buf;
                    for (auto& r : M.g) t += r + "\n";
                    firstB = t;
                }
            }
        }
    }
    printf("随机 %d 组：删法 A（DFS 后序）不合格 %d 组，删法 B（BFS 倒删）不合格 %d 组\n", N, badA, badB);
    if (!firstB.empty()) { printf("B 的最小反例：\n%s", firstB.c_str()); }
    return 0;
}
