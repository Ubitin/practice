// catalan_check.cpp   g++ -O2 -std=c++17
#include <bits/stdc++.h>
using namespace std;

int n;
long long dyck, noTouch;   // A：不越过对角线；B：内部不接触对角线

// x,y 当前坐标；k 已走步数；touch 内部接触对角线次数；above 是否曾走到 y>x
void dfs(int x, int y, int k, int touch, int above) {
    if (k == 2 * n) {
        if (x == n && y == n) {
            if (!above) ++dyck;        // 全程 y<=x
            if (!touch) ++noTouch;     // 内部无对角线点
        }
        return;
    }
    for (int s = 0; s < 2; ++s) {
        int nx = x + (s == 0), ny = y + (s == 1);
        if (nx > n || ny > n) continue;
        int nk = k + 1;
        int nt = touch + (nk < 2 * n && nx == ny ? 1 : 0);
        dfs(nx, ny, nk, nt, above || (ny > nx));
    }
}

int main() {
    for (n = 0; n <= 8; ++n) {
        dyck = noTouch = 0;
        dfs(0, 0, 0, 0, 0);
        printf("n=%d  Dyck=%lld  严格不接触=%lld\n", n, dyck, noTouch);
    }
}
