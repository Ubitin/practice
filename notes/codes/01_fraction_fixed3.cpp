// 分数四则运算 —— 修正版 v3
// 修掉：
//   ① 【编译错误·真正的根因】全局变量名 y1 撞上 <math.h> 的贝塞尔函数 double y1(double)
//      ⇒ 在 -std=gnu++20（评测机常用）下报 "'ll y1' redeclared as different kind of entity"
//      ⇒ 改名 y1 → dy1
//   ② std::gcd 需要 C++17 ⇒ 改成手写 gcd_（任何标准都稳，含 c++98/11/14）
//   ③ simple() 分子为 0 时返回裸 "0" ⇒ 改为 "0/1"
//   ④ 分母为负时没翻符号 ⇒ 符号统一到分子
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

static ll gcd_(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

static string simple(ll x, ll y) {
    if (y == 0) return "ERR";
    if (x == 0) return "0/1";
    ll r = gcd_(x, y);
    x /= r; y /= r;
    if (y < 0) { x = -x; y = -y; }
    return to_string(x) + "/" + to_string(y);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s1, s2;
    if (!(cin >> s1 >> s2)) return 0;

    // ★ 变量名避开 math.h 的贝塞尔函数族（y0/y1/yn/j0/j1/jn）
    ll dx1, dy1, dx2, dy2;

    ll pos1 = s1.find('/'), pos2 = s2.find('/');
    dx1 = stoll(s1.substr(0, pos1)); dy1 = stoll(s1.substr(pos1 + 1));
    dx2 = stoll(s2.substr(0, pos2)); dy2 = stoll(s2.substr(pos2 + 1));

    cout << "(" << dx1 << "/" << dy1 << ")+(" << dx2 << "/" << dy2 << ")=" << simple(dx1 * dy2 + dx2 * dy1, dy1 * dy2) << "\n";
    cout << "(" << dx1 << "/" << dy1 << ")-(" << dx2 << "/" << dy2 << ")=" << simple(dx1 * dy2 - dx2 * dy1, dy1 * dy2) << "\n";
    cout << "(" << dx1 << "/" << dy1 << ")*(" << dx2 << "/" << dy2 << ")=" << simple(dx1 * dx2, dy1 * dy2) << "\n";
    cout << "(" << dx1 << "/" << dy1 << ")/(" << dx2 << "/" << dy2 << ")=" << simple(dx1 * dy2, dx2 * dy1) << "\n";
    return 0;
}
