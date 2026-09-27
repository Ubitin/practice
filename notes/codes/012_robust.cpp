// 012 鲁棒版：自动适配「有没有 T」「范围多大」
//
// 背景：你的公式和精度都已经验证正确（样例对、边界对、n 到 1e10 与 Python 真值一致），
//       但两个版本都 WA ⇒ 最可能是【输入/输出格式约定】与题面不一致。
//       本版把常见约定差异一次性兼容掉：
//         · 先读一个整数 c，看它后面还剩多少个数：
//             - 若 c == 剩余个数 且 c 在合理范围内 → 认为第一行是组数 T（读 T 组）
//             - 否则                             → 认为第一行也是数据 n（读全部）
//         · 每题结果单独一行输出（最常见的约定）
//
// 精度：全程 __int128，n ≤ 1e18 无忧；若 n 可能 ≥ 1e19 请改用 012_bigint.cpp。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128;

static i128 sumMul(ll n, ll k) {
    ll t = (n - 1) / k;
    return (i128)k * t * (t + 1) / 2;
}

static string s128(i128 v) {
    if (v == 0) return "0";
    bool neg = v < 0; if (neg) v = -v;
    string s;
    while (v > 0) { s += char('0' + (int)(v % 10)); v /= 10; }
    if (neg) s += '-';
    reverse(s.begin(), s.end());
    return s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<ll> a;
    ll x;
    while (cin >> x) a.push_back(x);          // 先把所有数读进来
    if (a.empty()) return 0;

    vector<ll> qs;                            // 待计算的 n 列表
    size_t start = 0;
    size_t rest = a.size() - 1;
    if (a[0] > 0 && (size_t)a[0] == rest) {   // 第一行是组数 T
        qs.assign(a.begin() + 1, a.end());
        start = 1;
    } else {
        qs = a;                               // 没有 T，全是数据
    }
    (void)start;

    string out;
    for (size_t i = 0; i < qs.size(); ++i) {
        i128 r = sumMul(qs[i], 3) + sumMul(qs[i], 5) - sumMul(qs[i], 15);
        out += s128(r);
        out += '\n';
    }
    fputs(out.c_str(), stdout);
    return 0;
}
