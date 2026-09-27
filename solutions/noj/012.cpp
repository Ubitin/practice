#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128;

static i128 sumMul(ll n, ll k) {          // < n 的所有 k 的正倍数之和
    ll t = (n - 1) / k;                   // ★ "小于 n" 必须减 1
    return (i128)k * t * (t + 1) / 2;     // ★ 全程 __int128，先乘后除
}

static void print128(i128 v) {
    if (v == 0) { printf("0"); return; }
    if (v < 0) { printf("-"); v = -v; }
    char buf[64]; int p = 0;
    while (v > 0) { buf[p++] = char('0' + (int)(v % 10)); v /= 10; }
    while (p) putchar(buf[--p]);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll T;
    if (!(cin >> T)) return 0;
    while (T--) {
        ll n; cin >> n;
        print128(sumMul(n, 3) + sumMul(n, 5) - sumMul(n, 15));
        putchar('\n');
    }
    return 0;
}