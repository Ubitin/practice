// CF377A 随机数据生成器
// 关键：空格必须【连通】——做法是"从一个种子格不断往外长"，每加一格都和已有空格相邻
// 用法: cf377a_gen.exe <seed> [maxN] [maxM]
#include <bits/stdc++.h>
using namespace std;
using P = pair<int,int>;
int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};

int main(int argc, char** argv) {
    long long seed = argc > 1 ? atoll(argv[1]) : 1;
    int maxN = argc > 2 ? atoi(argv[2]) : 8;
    int maxM = argc > 3 ? atoi(argv[3]) : 8;
    mt19937_64 rng((unsigned long long)seed * 1000003ULL + 31ULL);
    auto rnd = [&](int hi) { return hi <= 0 ? 0 : (int)(rng() % (unsigned long long)(hi + 1)); };

    int n = 1 + rnd(maxN - 1), m = 1 + rnd(maxM - 1);
    vector<string> g(n, string(m, '#'));
    int x = rnd(n - 1), y = rnd(m - 1);
    g[x][y] = '.';
    vector<P> cells{{x, y}};
    int want = 1 + rnd(n * m - 1);                 // 想要多少个空格
    int guard = 0;
    while ((int)cells.size() < want && guard++ < n * m * 20) {
        auto [cx, cy] = cells[rnd((int)cells.size() - 1)];   // 随机挑一个已有空格
        int d = rnd(3), nx = cx + dx[d], ny = cy + dy[d];    // 往它的邻居长
        if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
        if (g[nx][ny] == '.') continue;
        g[nx][ny] = '.'; cells.push_back({nx, ny});
    }

    int E = (int)cells.size();
    int k = rnd(max(0, E - 1));                    // 至少留一个空格 ⇒ 一定有解
    printf("%d %d %d\n", n, m, k);
    for (auto& r : g) printf("%s\n", r.c_str());
    return 0;
}
