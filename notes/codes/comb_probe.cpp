// 组合数 4 种实现 + 边界与正确性实测（输出全部与 Python 真值对照）
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ================= ① 杨辉三角（精确） =================
vector<vector<ll>> pascal(ll nmax) {
    vector<vector<ll>> C(nmax + 1, vector<ll>(nmax + 1, 0));
    for (ll i = 0; i <= nmax; ++i) { C[i][0] = 1; for (ll j = 1; j <= i; ++j) C[i][j] = C[i-1][j-1] + C[i-1][j]; }
    return C;
}

// ================= ② 递推式（单个精确值） =================
ll C_iter(ll n, ll k) {
    if (k < 0 || k > n) return 0;
    k = min(k, n - k);
    ll r = 1;
    for (ll i = 1; i <= k; ++i) r = r * (n - k + i) / i;   // 先乘后除，且保证整除
    return r;
}

// ================= ③ 阶乘 + 逆元（模质数） =================
ll pw(ll a, ll b, ll m) { a %= m; ll r = 1 % m; while (b) { if (b&1) r = r*a%m; a = a*a%m; b >>= 1; } return r; }
struct CombMod {
    ll n, m; vector<ll> f, inv_f;
    CombMod(ll nmax, ll mod) : n(nmax), m(mod), f(nmax+1), inv_f(nmax+1) {
        f[0] = 1; for (ll i = 1; i <= n; ++i) f[i] = f[i-1]*i%m;
        inv_f[n] = pw(f[n], m-2, m);
        for (ll i = n; i >= 1; --i) inv_f[i-1] = inv_f[i]*i%m;
    }
    ll C(ll a, ll b) const { if (b<0||b>a) return 0; return f[a]*inv_f[b]%m*inv_f[a-b]%m; }
};

// ================= ④ 质因数分解 + 大数（精确，n 可较大） =================
// C(n,k) = n!/(k!(n-k)!)：把分子分母质因数分解，指数相减，再把 质数^指数 乘起来（大数用十进制数组）
struct Big { vector<int> d; Big(ll v = 1) { if (v == 0) d.push_back(0); while (v) { d.push_back(v%10); v/=10; } } };
static Big mulBig(const Big& a, const Big& b) {
    Big r; r.d.assign(a.d.size()+b.d.size(), 0);
    for (size_t i = 0; i < a.d.size(); ++i)
        for (size_t j = 0; j < b.d.size(); ++j) {
            int v = a.d[i]*b.d[j] + r.d[i+j];
            r.d[i+j] = v % 10; r.d[i+j+1] += v / 10;
        }
    while (r.d.size() > 1 && r.d.back() == 0) r.d.pop_back();
    return r;
}
static string strBig(const Big& a) { string s; for (int i = (int)a.d.size()-1; i >= 0; --i) s += char('0'+a.d[i]); return s; }
string C_exact(ll n, ll k) {
    if (k < 0 || k > n) return "0";
    map<ll,int> e;
    auto add = [&](ll x, int sgn) { for (ll p = 2; p*p <= x; ++p) while (x%p==0) { e[p]+=sgn; x/=p; } if (x>1) e[x]+=sgn; };
    for (ll i = 1; i <= n; ++i) add(i, +1);
    for (ll i = 1; i <= k; ++i) add(i, -1);
    for (ll i = 1; i <= n-k; ++i) add(i, -1);
    Big r(1);
    for (auto& kv : e) { if (kv.second <= 0) continue; for (int t = 0; t < kv.second; ++t) { Big p(kv.first); r = mulBig(r, p); } }
    return strBig(r);
}

int main() {
    printf("【A】long long 能装到多大的精确组合数（C(n, n/2)）:\n");
    for (ll n = 60; n <= 68; ++n) {
        ll c = C_iter(n, n/2);
        // 用 ④ 的精确值判断是否溢出：比较位数/数值
        string ex = C_exact(n, n/2);
        bool fits = (ex.size() < 19) || (ex.size() == 19 && ex <= "9223372036854775807");
        printf("  C(%2lld,%2lld) = %-22s  %s\n", n, n/2, ex.c_str(), fits ? "long long 装得下 ✅" : "溢出 ✘");
        (void)c;
    }

    printf("\n【B】① 杨辉 vs ② 递推：n ≤ 20 全部一致? ");
    { auto P = pascal(20); bool ok = true;
      for (ll n = 0; n <= 20; ++n) for (ll k = 0; k <= n; ++k) if (P[n][k] != C_iter(n,k)) ok = false;
      printf("%s\n", ok ? "是 ✅" : "否 ✘"); }
    printf("    ② 抽样: C(20,10)=%lld  C(30,15)=%lld  C(50,25)=%lld\n", C_iter(20,10), C_iter(30,15), C_iter(50,25));
    printf("    ④ 精确: C(50,25)=%s  C(66,33)=%s\n", C_exact(50,25).c_str(), C_exact(66,33).c_str());

    const ll MOD = 1000000007;
    CombMod cm(200, MOD);
    printf("\n【C】③ 阶乘+逆元 mod 1e9+7：\n");
    printf("    C(100,50) mod p = %lld\n", cm.C(100,50));
    { bool ok = true;
      for (ll n = 0; n <= 40; ++n) for (ll k = 0; k <= n; ++k) if (cm.C(n,k) != C_iter(n,k) % MOD) ok = false;
      printf("    与 ② 精确值取模对照（n ≤ 40）: %s\n", ok ? "全部一致 ✅" : "不一致 ✘"); }
    printf("    C(200,100) mod p = %lld\n", cm.C(200,100));

    // 反例：模数是合数时逆元法失效
    const ll MOD2 = 1000000006;                     // 合数 = 2 × 500000003
    CombMod cm2(200, MOD2);
    printf("\n【D】⚠️ 模数换成合数 1000000006 时，③ 立即失效：\n");
    printf("    ③ 给的 C(100,50) mod m = %lld\n", cm2.C(100,50));
    printf("    ④ 精确值 C(100,50) = %s\n", C_exact(100,50).c_str());
    printf("    （把上面的精确值对 1000000006 取模，与 ③ 的结果比对即可看出错；见 Python 校验输出）\n");

    auto t0 = chrono::steady_clock::now();
    ll acc = 0; for (ll i = 0; i < 1000000; ++i) acc += cm.C(100, i % 101);
    auto t1 = chrono::steady_clock::now();
    printf("\n【E】速度：100 万次 C(100,k) 查询 = %lld ms（acc=%lld）\n",
           (ll)chrono::duration_cast<chrono::milliseconds>(t1-t0).count(), acc);
    return 0;
}
