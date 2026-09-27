// 012 诊断版：既能交、也能帮你定位 WA
//
// 用法：直接提交。它会把每一步算出的数【另外】写到 stderr（DevForge 判题只看 stdout，
//       stderr 不影响判分），所以你本地跑的时候能看到每一组算的是什么。
//
// 兼容性：
//   · 第一行是组数 T  → 读 T 组
//   · 第一行就是数据 n → 读全部（直到 EOF）
//   · 单组单个 n      → 也对
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128;

static i128 sumMul(ll n, ll k) {            // < n 的所有 k 的正倍数之和
    ll t = (n - 1) / k;                     // ★ 小于 n ⇒ 减 1
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

    vector<ll> a; ll x;
    while (cin >> x) a.push_back(x);
    if (a.empty()) return 0;

    vector<ll> qs;
    if (a[0] > 0 && (size_t)a[0] == a.size() - 1) {   // 第一行是组数 T
        qs.assign(a.begin() + 1, a.end());
        fprintf(stderr, "[诊断] 判定为「有 T」，T=%lld，共 %zu 组\n", a[0], qs.size());
    } else {                                          // 没有 T
        qs = a;
        fprintf(stderr, "[诊断] 判定为「没有 T」，共 %zu 个数\n", qs.size());
    }

    string out;
    for (size_t i = 0; i < qs.size(); ++i) {
        ll n = qs[i];
        i128 r3 = sumMul(n, 3), r5 = sumMul(n, 5), r15 = sumMul(n, 15);
        i128 ans = r3 + r5 - r15;
        fprintf(stderr, "[诊断] 第%zu组 n=%lld : S3=%s S5=%s S15=%s -> ans=%s\n",
                i + 1, n, s128(r3).c_str(), s128(r5).c_str(), s128(r15).c_str(), s128(ans).c_str());
        out += s128(ans);
        out += '\n';
    }
    fputs(out.c_str(), stdout);
    return 0;
}
