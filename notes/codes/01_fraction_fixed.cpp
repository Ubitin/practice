// 分数四则运算 —— 修正版（修掉 substr 丢返回值、!x%i、负数、0 分子、除数为 0）
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

static ll gcdll(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

// 化简：符号统一到分子；分母 0 → "ERR"；分子 0 → "0/1"
static string simple(ll x, ll y) {
    if (y == 0) return "ERR";
    if (x == 0) return "0/1";
    ll g = gcdll(x, y);
    x /= g; y /= g;
    if (y < 0) { x = -x; y = -y; }
    return to_string(x) + "/" + to_string(y);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s1, s2;
    if (!(cin >> s1 >> s2)) return 0;
    ll p1 = s1.find('/'), p2 = s2.find('/');
    ll a = stoll(s1.substr(0, p1)), b = stoll(s1.substr(p1 + 1));   // ★ 关键：赋值回去
    ll c = stoll(s2.substr(0, p2)), d = stoll(s2.substr(p2 + 1));

    cout << "(" << a << "/" << b << ")+(" << c << "/" << d << ")=" << simple(a * d + c * b, b * d) << "\n";
    cout << "(" << a << "/" << b << ")-(" << c << "/" << d << ")=" << simple(a * d - c * b, b * d) << "\n";
    cout << "(" << a << "/" << b << ")*(" << c << "/" << d << ")=" << simple(a * c, b * d) << "\n";
    cout << "(" << a << "/" << b << ")/(" << c << "/" << d << ")=" << simple(a * d, b * c) << "\n";
    return 0;
}
