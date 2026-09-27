// 分数四则运算 —— 修正版 v2
// 修掉的问题：
//   ① 【编译错误】std::gcd 需要 C++17 ⇒ 改成自己写的 gcd_（任何 -std 都能过，含 c++98/11/14）
//   ② simple() 返回裸 "0" ⇒ 改为 "0/1"（分母不能丢）
//   ③ 分母为负时没翻符号 ⇒ 统一把符号提到分子（输出 -1/2 而不是 1/-2）
//   ④ 顺带：解析用 substr 的返回值（原版漏接导致分母丢失）
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 自己写的 gcd：不依赖 C++17，任何标准都能编译
static ll gcd_(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

// 化简：符号统一到分子；分子 0 输出 0/1；分母 0 输出 ERR
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

    ll pos1 = s1.find('/'), pos2 = s2.find('/');
    ll x1 = stoll(s1.substr(0, pos1)), y1 = stoll(s1.substr(pos1 + 1));
    ll x2 = stoll(s2.substr(0, pos2)), y2 = stoll(s2.substr(pos2 + 1));

    cout << "(" << x1 << "/" << y1 << ")+(" << x2 << "/" << y2 << ")=" << simple(x1 * y2 + x2 * y1, y1 * y2) << "\n";
    cout << "(" << x1 << "/" << y1 << ")-(" << x2 << "/" << y2 << ")=" << simple(x1 * y2 - x2 * y1, y1 * y2) << "\n";
    cout << "(" << x1 << "/" << y1 << ")*(" << x2 << "/" << y2 << ")=" << simple(x1 * x2, y1 * y2) << "\n";
    cout << "(" << x1 << "/" << y1 << ")/(" << x2 << "/" << y2 << ")=" << simple(x1 * y2, x2 * y1) << "\n";
    return 0;
}
