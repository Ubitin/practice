// ============================================================
//  01_p1102_fixed.cpp —— 洛谷 P1102 A-B 数对（修正·优化版）
//
//  原文件 test_/01.cpp 保持原样、未做任何改动。
//
//  题目：给出 N 个正整数和一个正整数 C，求满足 A - B = C 的数对个数
//        （不同位置的数字算不同数对）。
//
//  原版为什么 TLE（三层叠加，全部实测于 01_p1102_bench.cpp）：
//   ① 循环里 vector<ll> u = v;        每次 O(n) 拷贝，循环 n 次 → O(n²)
//   ② u.erase(...)                    每次 O(n) 搬移          → O(n²)
//   ③ ll fi(ll x, vector<ll> v) 传值  每次调用再拷一次         → O(n²)
//   ④ fi 内部自己扫区间计数            O(出现次数)，非常数时间
//   实测：拷贝元素总数 / n^2 恒为常数 2.00 → 铁证 O(n²)；
//         n=2*10^5 时约 8*10^10 次 long long 拷贝 ≈ 0.64 TB memcpy。
//
//  本版思路：
//   · 题面 C 是【正整数】→ A - B = C ≥ 1 蕴含 A > B，A 和 B 必然落在不同位置，
//     所以【不需要 erase】，也不需要最后 /2；
//   · 排序后对每个 B，二分统计值为 B+C 的元素个数（upper_bound - lower_bound），
//     一次 O(log n)，总共 O(n log n)；
//   · 所有参数用 const 引用传递，杜绝拷贝。
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, C;
    if (!(cin >> n >> C)) return 0;

    vector<ll> a(n);
    for (ll i = 0; i < n; ++i) cin >> a[i];
    sort(a.begin(), a.end());

    ll ans = 0;
    for (ll i = 0; i < n; ++i) {
        ll t = a[i] + C;                       // 找一个 A = B + C
        // 统计 t 在 a 中出现的次数：一次 O(log n)，不要自己扫区间
        ans += upper_bound(a.begin(), a.end(), t)
             - lower_bound(a.begin(), a.end(), t);
    }
    cout << ans << '\n';
    return 0;
}

// ------------------------------------------------------------
// 附：另外两种等价写法（同样 AC，按喜好选一种）
//
// 【双指针 / 尺取法】排序 O(n log n) + 扫描 O(n)，常数比二分小约 2 倍
//   sort(a.begin(), a.end());
//   ll ans = 0, j = 0, k = 0;
//   for (ll i = 0; i < n; ++i) {
//       ll t = a[i] + C;
//       if (j < i) j = i;
//       if (k < i) k = i;
//       while (j < n && a[j] <  t) ++j;      // j = 第一个 >= t
//       while (k < n && a[k] <= t) ++k;      // k = 第一个 >  t
//       ans += k - j;                        // 区间 [j,k) 全是 t
//   }
//
// 【哈希计数】期望 O(n)（值域大时最快；值域小时也很快）
//   unordered_map<ll, ll> cnt;
//   cnt.reserve(n * 2);                      // 防 rehash
//   for (ll x : a) cnt[x]++;
//   ll ans = 0;
//   for (auto& p : cnt) {
//       auto it = cnt.find(p.first + C);
//       if (it != cnt.end()) ans += p.second * it->second;
//   }
//   // ⚠️ 千万别写 cnt[p.first + C]：operator[] 在缺失时会【插入】新元素，
//   //    对 unordered_map 会触发 rehash 使迭代器失效（UB），
//   //    对 map 会一边遍历一边加元素 → 死循环。必须用 find。
// ------------------------------------------------------------
