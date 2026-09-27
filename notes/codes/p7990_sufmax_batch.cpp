#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main(){
    ios::sync_with_stdio(false); cin.tie(nullptr);
    ll n;
    while (cin >> n) {
        vector<ll> a(n + 1), t(n + 1, 0);
        for (ll i = 1; i <= n; ++i) cin >> a[i];
        ll q; cin >> q;
        vector<ll> mon(q + 2, 0);
        for (ll i = 1; i <= q; ++i) {
            ll k; cin >> k;
            if (k == 1) { ll p, x; cin >> p >> x; a[p] = x; t[p] = i; }
            else        { ll x; cin >> x; mon[i] = x; }
        }
        vector<ll> suf(q + 2, 0);
        for (ll i = q; i >= 0; --i) suf[i] = max(suf[i + 1], mon[i]);
        for (ll i = 1; i <= n; ++i) cout << max(a[i], suf[t[i]]) << " \n"[i == n];
    }
    return 0;
}