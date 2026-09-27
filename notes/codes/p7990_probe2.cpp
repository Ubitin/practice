// 探针 v2：内部用两套独立算法同时算，并直接把两套结果与"打印值"一起输出，便于对照
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ll n;
    if (!(cin >> n)) return 0;
    vector<ll> a0(n + 1);
    for (ll i = 1; i <= n; ++i) cin >> a0[i];
    ll q; cin >> q;
    vector<ll> typ(q), p(q, 0), x(q, 0);
    for (ll i = 0; i < q; ++i) {
        cin >> typ[i];
        if (typ[i] == 1) cin >> p[i] >> x[i];
        else             cin >> x[i];
    }

    // ── 算法 A：倒扫，求"最后一次覆盖之后的最大补助" ──
    vector<ll> aA = a0, sub(n + 1, 0);
    vector<char> seen(n + 1, 0);
    ll mx = 0;
    for (ll i = q - 1; i >= 0; --i) {
        if (typ[i] == 2) mx = max(mx, x[i]);
        else if (!seen[p[i]]) { seen[p[i]] = 1; sub[p[i]] = mx; aA[p[i]] = x[i]; }
    }
    vector<ll> resA(n + 1);
    for (ll i = 1; i <= n; ++i) resA[i] = max(aA[i], sub[i]);

    // ── 算法 B：对每个人单独扫一遍后缀（O(nq)，绝对直白，不可能写错）──
    vector<ll> lastSet(n + 1, -1), lastVal(n + 1, 0);
    for (ll i = 1; i <= n; ++i) lastVal[i] = a0[i];
    for (ll i = 0; i < q; ++i)
        if (typ[i] == 1) { lastSet[p[i]] = i; lastVal[p[i]] = x[i]; }
    vector<ll> resB(n + 1);
    for (ll i = 1; i <= n; ++i) {
        ll best = lastVal[i];
        for (ll j = lastSet[i] + 1; j < q; ++j)      // 只看"最后一次覆盖之后"的事件
            if (typ[j] == 2) best = max(best, x[j]);
        resB[i] = best;
    }

    bool same = true;
    for (ll i = 1; i <= n; ++i) if (resA[i] != resB[i]) same = false;
    printf("A(倒扫):"); for (ll i = 1; i <= n; ++i) printf(" %lld", resA[i]);
    printf("\nB(直白):"); for (ll i = 1; i <= n; ++i) printf(" %lld", resB[i]);
    printf("\n两套算法一致: %s\n", same ? "是" : "否 ✘");
    return 0;
}
