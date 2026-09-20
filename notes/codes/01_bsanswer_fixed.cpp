// ============================================================
//  01_bsanswer_fixed.cpp —— 二分答案：补上 main + 修正二分写法
//
//  原文件 test_/01.cpp 保持原样、未做任何改动。
//
//  这个文件看起来是 P1873「砍树」：锯片高度 H，得到的木材
//     W(H) = Σ max(0, h_i − H)，求满足 W(H) ≥ M 的【最大整数 H】。
//
//  原码的问题（按严重程度）：
//   ★① 没有 main 函数 → 链接期报 undefined reference to `WinMain`
//   ★② 数组从未排序 → upper_bound 要求有序，否则结果无意义
//   ★③ mid = (l+r)>>1 写在 while 外面，循环里从不更新
//        → l = mid+1 每轮赋同一个值 → 满足 l<=r 时【死循环】
//   ★④ l = mid + 1 写在 if 外面，无条件执行；true 分支里已经改过 r
//        → 两个边界同时被改，逻辑自相矛盾
//   ★⑤ 二分的是【下标】而不是【值域】——答案可能根本不在数组里！
//        反例：n=2, a={20,15}, M=7
//              W(14)=6+1=7 ≥ 7 ✔，W(15)=5+0=5 < 7 ✘ → 答案是 14
//              但 14 不在 {20,15} 中，下标域二分永远找不到它
//   ☆⑥ p() 里把 l 当区间左端点传给 upper_bound：木材总量必须统计
//        所有高于 H 的树；当 l > mid 时会漏掉 (mid, l) 之间的树
//   ☆⑦ 两个函数都用 vector<long long> v【按值传递】→ 每次调用 O(n) 拷贝
//   ☆⑧ cout<<ans 写在 find() 里、函数却 return 0；函数名 find 还会遮蔽 std::find
//
//  正确做法：二分【值域】[0, max(h)]，mid 在循环内重新计算。
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll n, m;
vector<ll> a;

// 锯片高度 H 能否得到至少 m 的木材
bool ok(ll H) {
    ll sum = 0;
    for (ll x : a) {
        if (x > H) {
            sum += x - H;
            if (sum >= m) return true;   // 提前返回，既快又防溢出
        }
    }
    return sum >= m;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n >> m)) return 0;
    a.resize(n);
    for (ll i = 0; i < n; ++i) cin >> a[i];

    ll lo = 0, hi = *max_element(a.begin(), a.end());   // ★ 二分的是值域，不是下标
    while (lo < hi) {
        ll mid = lo + (hi - lo + 1) / 2;   // ★ mid 必须在循环内算；+1 是上取整防死循环
        if (ok(mid)) lo = mid;             // 可行 → 试更大的
        else         hi = mid - 1;         // 不可行 → 往小
    }
    cout << lo << '\n';
    return 0;
}

// ------------------------------------------------------------
// 二分答案的两套模板（背这两条就够）
//
// 【找最后一个满足 check 的值】（本文件用的）
//   ll lo = 最小值, hi = 最大值;
//   while (lo < hi) {
//       ll mid = lo + (hi - lo + 1) / 2;   // ⚠️ 上取整，否则 lo<hi 时死循环
//       if (check(mid)) lo = mid;
//       else            hi = mid - 1;
//   }
//   cout << lo;
//
// 【找第一个满足 check 的值】
//   ll lo = 最小值, hi = 最大值;
//   while (lo < hi) {
//       ll mid = lo + (hi - lo) / 2;       // 下取整
//       if (check(mid)) hi = mid;
//       else            lo = mid + 1;
//   }
//   cout << lo;
//
// ⚠️ 三条铁律：
//   1) mid 必须在循环体内计算，写在外面就是死循环
//   2) 上取整模板配 lo=mid，下取整模板配 hi=mid，配错就死循环
//   3) check 的二分域是【答案的取值域】，不是数组下标
// ------------------------------------------------------------
