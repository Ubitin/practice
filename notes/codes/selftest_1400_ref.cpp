// 自测用参考实现（全部经暴力对拍验证）
// 用法：selftest2.exe <题号> < 输入
//   1 = 等值对计数（排序 + 双指针/带下标二分）
//   2 = 容器灌水（二分答案）
//   3 = 区间和奇偶查询（前缀和）
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ── 题 1：数出有多少对 (i<j) 满足 a[i] + a[j] == 0 ──
// 正确公式：先按值计数，再对每个值 v 配对 (-v)；v != -v 时贡献 cnt[v]*cnt[-v]，
//           全部求和后 /2（每对算了两遍）；v == -v（即 v==0）时贡献 C(cnt[0], 2)。
ll countPairs(const vector<ll>& a) {
    map<ll, ll> c;
    for (ll x : a) ++c[x];
    ll ans = 0;
    for (auto& kv : c) {
        ll v = kv.first, n = kv.second;
        if (v > 0) continue;                    // 只在 v <= 0 时处理，避免重复
        if (v == 0) ans += n * (n - 1) / 2;
        else {
            auto it = c.find(-v);
            if (it != c.end()) ans += n * it->second;
        }
    }
    return ans;
}

// ── 题 3：前缀和 + 奇偶 ──
// 询问：把 [l,r] 每个数都改成 k 之后，整个数组的和是奇数吗？
// 原和 S。新和 = S - (pre[r]-pre[l-1]) + k*(r-l+1)
int solve3() {
    ll t; if (!(cin >> t)) return -1;
    while (t--) {
        ll n, q; cin >> n >> q;
        vector<ll> pre(n + 1, 0);
        for (ll i = 1; i <= n; ++i) { ll x; cin >> x; pre[i] = pre[i - 1] + x; }
        while (q--) {
            ll l, r, k; cin >> l >> r >> k;
            ll sum = pre[n] - (pre[r] - pre[l - 1]) + k * (r - l + 1);
            cout << ((sum & 1LL) ? "YES" : "NO") << "\n";
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    int which = (argc > 1) ? atoi(argv[1]) : 1;
    if (which == 1) {
        ll n; if (!(cin >> n)) return 0;
        vector<ll> a(n);
        for (ll i = 0; i < n; ++i) cin >> a[i];
        cout << countPairs(a) << "\n";
    } else if (which == 2) {
        ll t; if (!(cin >> t)) return 0;
        while (t--) {
            ll n, w; cin >> n >> w;
            vector<ll> a(n);
            for (ll i = 0; i < n; ++i) cin >> a[i];
            ll lo = 0, hi = 2000000000LL;
            while (lo < hi) {
                ll mid = lo + (hi - lo + 1) / 2;
                __int128 need = 0;
                for (ll i = 0; i < n; ++i) if (a[i] < mid) need += (__int128)mid - a[i];
                if (need <= (__int128)w) lo = mid; else hi = mid - 1;
            }
            cout << lo << "\n";
        }
    } else {
        solve3();
    }
    return 0;
}
