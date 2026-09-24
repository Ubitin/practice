// CF1886B Fear of the Dark —— 修正版（保留你原来的"二分 + p(w) 判定"骨架）
// 相对 test_\01.cpp 只有三处改动，都用 ★ 标出
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const double del = 1e-11;
ll px, py, ax, ay, bx, by;

double dis(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

bool p(double w) {
    // ★1 这里原来是 (ax*ax + ay*ay) —— 那是【平方距离】，却拿去和 w 比，量纲不一致
    if (dis(0, 0, ax, ay) > (w + del) && dis(0, 0, bx, by) > (w + del)) return false;
    if (dis(ax, ay, px, py) > (w + del) && dis(bx, by, px, py) > (w + del)) return false;
    if (!((dis(0, 0, ax, ay) < (w - del) && dis(px, py, ax, ay) < (w - del)) ||
          (dis(0, 0, bx, by) < (w - del) && dis(px, py, bx, by) < (w - del)))) {
        if (dis(ax, ay, bx, by) > (2 * w + del)) return false;
    }
    return true;
}

double find(double l, double r) {
    // ★2 原来是 while (l <= r - del) —— 浮点下 r-del 会被舍入，可能永远不小于 l → 死循环
    double ans = r;
    for (int it = 0; it < 100; ++it) {          // 固定 100 次，区间缩到 1e6/2^100，永不死循环
        double mid = (l + r) / 2;
        if (p(mid)) { ans = mid; r = mid; }
        else l = mid;
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll t = 0;
    cin >> t;
    while (t--) {
        cin >> px >> py >> ax >> ay >> bx >> by;
        double l = 0, r = 4000;                 // ★3 原来 l = 0.1：答案可能是 0，下界必须给 0
        printf("%.10f\n", find(l, r));
    }
    return 0;
}
