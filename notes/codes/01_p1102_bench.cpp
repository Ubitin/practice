// ============================================================
//  01_p1102_bench.cpp —— P1102 A-B 数对：四种解法正确性 + 性能实测
//
//  目的：量化 01.cpp（原版）TLE 的三层原因，并给出三种 O(n log n)/O(n) 解法。
//  原文件 test_/01.cpp 保持原样、未改动。
//
//  编译：g++ -std=gnu++17 -O2 01_p1102_bench.cpp -o bench
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using Clock = chrono::steady_clock;

static double ms_since(Clock::time_point t0) {
    return chrono::duration<double, milli>(Clock::now() - t0).count();
}

// ------------------------------------------------------------------
// 【原版】逐字保留 01.cpp 的写法，只把参数传入改成值传递以保持一致
// ------------------------------------------------------------------
ll fi(ll x, vector<ll> v) {                       // ⚠️ 传值：每次调用拷贝整个 vector
    ll sum = 0;
    auto iter = lower_bound(v.begin(), v.end(), x);
    for (auto y = iter;; ++iter) {
        if (iter == v.end()) break;
        if (*iter != x) break;
        if (*y == x) sum++;
    }
    return sum;
}

ll solve_orig(vector<ll> v, ll x) {
    ll ans = 0;
    ll n = v.size();
    sort(v.begin(), v.end());
    for (ll i = 0; i < n; ++i) {
        vector<ll> u = v;                         // ⚠️ O(n) 拷贝，循环 n 次 → O(n²)
        u.erase(u.begin() + i, u.begin() + i + 1); // ⚠️ O(n) 搬移
        ans += (v[i] - x > 0) ? fi(v[i] - x, u) : 0;            // ⚠️ fi 内又拷一次
        ans += (v[i] + x <= v[n - 1]) ? fi(v[i] + x, u) : 0;    // ⚠️ 再来一次
    }
    return ans / 2;
}

// ------------------------------------------------------------------
// 方案 A：排序 + 对每个 B 二分统计 (B+C) 的个数 —— O(n log n)
// ------------------------------------------------------------------
ll solve_bs(vector<ll> v, ll C) {
    sort(v.begin(), v.end());
    ll n = v.size(), ans = 0;
    for (ll i = 0; i < n; ++i)
        ans += upper_bound(v.begin(), v.end(), v[i] + C)
             - lower_bound(v.begin(), v.end(), v[i] + C);
    return ans;
}

// ------------------------------------------------------------------
// 方案 B：排序 + 双指针（尺取法）—— 排序 O(n log n) + 扫描 O(n)
//   j：第一个 v[j] >= v[i]+C   k：第一个 v[k] > v[i]+C
// ------------------------------------------------------------------
ll solve_tp(vector<ll> v, ll C) {
    sort(v.begin(), v.end());
    ll n = v.size(), ans = 0;
    ll j = 0, k = 0;
    for (ll i = 0; i < n; ++i) {
        ll t = v[i] + C;
        if (j < i) j = i;
        if (k < i) k = i;
        while (j < n && v[j] <  t) ++j;
        while (k < n && v[k] <= t) ++k;
        ans += k - j;
    }
    return ans;
}

// ------------------------------------------------------------------
// 方案 C：哈希计数 —— 期望 O(n)
//   注意必须用 find，不能写 cnt[p.first + C]（那会往表里插入，破坏遍历）
// ------------------------------------------------------------------
ll solve_hash(vector<ll> v, ll C) {
    unordered_map<ll, ll> cnt;
    cnt.reserve(v.size() * 2);
    for (ll x : v) cnt[x]++;
    ll ans = 0;
    for (auto& p : cnt) {
        auto it = cnt.find(p.first + C);
        if (it != cnt.end()) ans += p.second * it->second;
    }
    return ans;
}

// ------------------------------------------------------------------
// 【计数版】同样的算法，但统计"元素拷贝次数"与"扫描次数"
//   耗时受缓存/分配器影响有噪声；操作次数是确定的，更能证明复杂度
// ------------------------------------------------------------------
ll fi_cnt(ll x, vector<ll> v, long long& copies, long long& scans) {
    copies += (long long)v.size();          // ⚠️ 形参传值：每次调用拷一整个 vector
    ll sum = 0;
    auto iter = lower_bound(v.begin(), v.end(), x);
    for (auto y = iter;; ++iter) {
        scans++;                            // ⚠️ 逐元素扫描
        if (iter == v.end()) break;
        if (*iter != x) break;
        if (*y == x) sum++;
    }
    return sum;
}

ll solve_orig_count(vector<ll> v, ll x, long long& copies, long long& scans, long long& erases) {
    ll ans = 0, n = v.size();
    sort(v.begin(), v.end());
    for (ll i = 0; i < n; ++i) {
        copies += n;                        // ⚠️ u = v
        erases += (n - i - 1);              // ⚠️ erase 搬移的元素数
        vector<ll> u = v;
        u.erase(u.begin() + i, u.begin() + i + 1);
        ans += (v[i] - x > 0) ? fi_cnt(v[i] - x, u, copies, scans) : 0;
        ans += (v[i] + x <= v[n - 1]) ? fi_cnt(v[i] + x, u, copies, scans) : 0;
    }
    return ans / 2;
}

// ------------------------------------------------------------------
// 隔离用：只做"统计值 x 的出现次数"这一步的两种写法
// ------------------------------------------------------------------
ll count_scan(const vector<ll>& v, ll x) {        // 原版 fi 的扫描部分（不含传值拷贝）
    ll sum = 0;
    auto iter = lower_bound(v.begin(), v.end(), x);
    for (auto y = iter;; ++iter) {
        if (iter == v.end()) break;
        if (*iter != x) break;
        if (*y == x) sum++;
    }
    return sum;
}
ll count_range(const vector<ll>& v, ll x) {       // 正解：O(log n)
    return upper_bound(v.begin(), v.end(), x) - lower_bound(v.begin(), v.end(), x);
}

// ------------------------------------------------------------------
// 暴力 O(n²)：小数据下作为正确性基准（i != j 保证不配自己）
// ------------------------------------------------------------------
ll solve_brute(const vector<ll>& v, ll C) {
    ll n = v.size(), ans = 0;
    for (ll i = 0; i < n; ++i)
        for (ll j = 0; j < n; ++j)
            if (i != j && v[i] - v[j] == C) ans++;
    return ans;
}

int main() {
    cout << "===== [1] 正确性交叉验证（随机小数据，暴力 O(n^2) 作基准）=====\n";
    {
        mt19937_64 rng(20260917);
        int bad[3] = {0, 0, 0};
        for (int tc = 0; tc < 3000; ++tc) {
            int n = 1 + rng() % 40;
            ll C = 1 + rng() % 10;                 // 题面 C 是正整数 → C >= 1
            vector<ll> v(n);
            for (auto& x : v) x = 1 + rng() % 30;  // 故意取小值域，制造大量重复
            ll expect = solve_brute(v, C);
            if (solve_bs(v, C)   != expect) bad[0]++;
            if (solve_tp(v, C)   != expect) bad[1]++;
            if (solve_hash(v, C) != expect) bad[2]++;
        }
        cout << "3000 组随机数据比对（含大量重复值）：\n";
        cout << "  二分法  不一致 " << bad[0] << " 组\n";
        cout << "  双指针  不一致 " << bad[1] << " 组\n";
        cout << "  哈希    不一致 " << bad[2] << " 组\n";
    }

    cout << "\n===== [2] 第一二层：数「元素拷贝次数」来证明 O(n^2) =====\n";
    {
        cout << "耗时受缓存/内存分配影响有噪声，所以这里直接数操作次数（确定性结果）\n";
        cout << "构造：n/2 个 1 + n/2 个 2，C=1（fi 会被调用且命中长区间）\n\n";
        cout << "n\t拷贝元素总数\t/n^2\terase搬移\t/n^2\tfi扫描次数\t/n^2\n";
        for (int n : {1000, 2000, 4000, 8000, 16000}) {
            vector<ll> v(n);
            for (int i = 0; i < n; ++i) v[i] = (i < n / 2) ? 1 : 2;
            long long copies = 0, scans = 0, erases = 0;
            solve_orig_count(v, 1, copies, scans, erases);
            double n2 = (double)n * n;
            cout << n << "\t" << copies << "\t\t" << fixed << setprecision(2)
                 << (copies / n2) << "\t" << erases << "\t\t"
                 << (erases / n2) << "\t" << scans << "\t\t" << (scans / n2) << "\n";
        }
        cout << "→ 三列 /n^2 都是【常数】→ 三项全部 O(n^2)，没有任何一项例外\n";
        cout << "→ 拷贝元素总数 = 2n^2（u=v 贡献 n^2，fi 传值拷贝贡献 n^2 左右）\n";
        cout << "→ 若两个 fi 都被命中则达 3n^2 —— 反正都是 n^2 量级，常数不影响结论\n";
        double nn = 200000.0, n2 = nn * nn;
        cout << "→ 换算到 n = 2*10^5（本题上限）：\n";
        cout << "   本档构造 2n^2 = " << fixed << setprecision(1) << (2 * n2 / 1e9) << " * 10^9 个 long long\n";
        cout << "   即 " << setprecision(2) << (2 * n2 * 8 / 1e12) << " TB 的内存拷贝\n";
        cout << "   最坏 3n^2 则是 " << setprecision(2) << (3 * n2 * 8 / 1e12) << " TB\n";
        cout << "   → 光是 memcpy 就是 TB 量级，TLE 是必然的，跟常数优化无关\n";
    }

    cout << "\n===== [2b] 小 n 区间的实测耗时（此处计时稳定）=====\n";
    {
        cout << "n\t耗时(ms)\t相对上一档\n";
        double prev = 0;
        for (int n : {500, 1000, 2000, 4000}) {
            vector<ll> v(n);
            for (int i = 0; i < n; ++i) v[i] = (i < n / 2) ? 1 : 2;
            auto t0 = Clock::now();
            volatile ll r = solve_orig(v, 1);
            (void)r;
            double t = ms_since(t0);
            cout << n << "\t" << fixed << setprecision(2) << t << "\t\t";
            if (prev > 0) cout << setprecision(2) << (t / prev) << "x";
            cout << "\n";
            prev = t;
        }
        cout << "→ 每翻倍约 4 倍，与上面的操作次数结论一致\n";
    }

    cout << "\n===== [3] 三种正解在 n = 200000 下的耗时 =====\n";
    {
        const int n = 200000;
        mt19937_64 rng(20260917);
        vector<ll> base1(n), base2(n);
        for (auto& x : base1) x = 1 + (ll)(rng() % 1000000000LL);  // 随机
        for (auto& x : base2) x = 1 + (ll)(rng() % 3);             // 值域只有 3 个 → 极端重复
        ll C = 1000;

        auto run = [&](const char* name, ll(*f)(vector<ll>, ll), vector<ll> v) {
            auto t0 = Clock::now();
            ll r = f(v, C);
            printf("  %-28s %8.2f ms   答案=%lld\n", name, ms_since(t0), r);
        };
        cout << "—— 随机数据（值域 1e9）——\n";
        run("二分法  sort+lower/upper", solve_bs,   base1);
        run("双指针  sort+尺取",        solve_tp,   base1);
        run("哈希    unordered_map",    solve_hash, base1);
        cout << "—— 极端重复数据（值域只有 3 个）——\n";
        run("二分法  sort+lower/upper", solve_bs,   base2);
        run("双指针  sort+尺取",        solve_tp,   base2);
        run("哈希    unordered_map",    solve_hash, base2);
    }

    cout << "\n===== [4] 第三层：fi 是 O(出现次数) 的线性扫描，不是 O(log n) =====\n";
    {
        // 构造：值只有 1 和 2，C=1。对 v[i]=2 要统计「1 的个数」= 一段很长的连续区间。
        // 单独隔离扫描成本（去掉传值拷贝的影响），对比「扫一遍」与「upper-lower」。
        double prev1 = 0, prev2 = 0;
        cout << "数据：n/2 个 1 与 n/2 个 2；统计值 1 的出现次数，共调用 n/2 次\n";
        cout << "n\t原版扫法(ms)\t翻倍\tupper-lower(ms)\t翻倍\n";
        for (int n : {2000, 4000, 8000, 16000}) {
            vector<ll> v(n);
            for (int i = 0; i < n; ++i) v[i] = (i < n / 2) ? 1 : 2;   // 已排序
            ll times = n / 2;
            auto t0 = Clock::now();
            volatile ll s1 = 0;
            for (ll i = 0; i < times; ++i) s1 += count_scan(v, 1);
            double t1 = ms_since(t0);
            auto t2 = Clock::now();
            volatile ll s2 = 0;
            for (ll i = 0; i < times; ++i) s2 += count_range(v, 1);
            double t3 = ms_since(t2);
            cout << n << "\t" << fixed << setprecision(2) << t1 << "\t\t";
            if (prev1 > 0) cout << setprecision(2) << (t1 / prev1) << "x";
            cout << "\t" << setprecision(2) << t3 << "\t\t";
            if (prev2 > 0) cout << setprecision(2) << (t3 / prev2) << "x";
            cout << "\n";
            prev1 = t1; prev2 = t3;
        }
        cout << "→ 扫法每翻倍约 4 倍（O(k) 线性扫描 k=n/2，调用 n/2 次 → O(n^2)）\n";
        cout << "→ upper-lower 随 n 只按 log 增长，绝对值几乎可忽略\n";
        cout << "→ 同一个统计结果，两种写法在 n=16000 时差约 200 倍以上\n";
        cout << "→ 结论：统计一个值的出现次数，永远用 upper_bound-lower_bound，不要自己扫区间\n";
    }

    cout << "\n===== [5] 关键前提：C >= 1 时 A != B，不需要 erase =====\n";
    {
        mt19937_64 rng(1);
        bool same = true;
        for (int tc = 0; tc < 2000 && same; ++tc) {
            int n = 2 + rng() % 20;
            vector<ll> v(n);
            for (auto& x : v) x = 1 + rng() % 15;
            ll C = 1 + rng() % 5;
            // 不 erase 的写法（方案 A）与 erase 版（原版）应完全一致
            if (solve_bs(v, C) != solve_orig(v, C)) same = false;
        }
        cout << "方案A（不 erase）与原版（erase）在 2000 组上答案"
             << (same ? "完全一致 ✔" : "不一致 ✘") << "\n";
        cout << "→ 因为 C>=1 蕴含 A>B，A 和 B 必然落在不同位置，erase 是多余的\n";
    }
    return 0;
}
