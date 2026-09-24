// CF1006E Military Problem —— 按你自己平时的 DFS 风格写（全局数组 + void dfs(ll u) + 1-based）
// 用法（本地编译时务必加栈参数，原因见文件末尾注释）：
//   g++ -O2 -std=c++17 -Wl,--stack,268435456 -o a.exe cf1006e_style_dfs.cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll maxn = 200005;

ll n, q;
ll par_[maxn];              // 父亲（和你平时一样，1-based）
vector<ll> ch[maxn];        // ch[u] = u 的直接手下，按名单顺序

ll pos_[maxn];              // 我第几个收到命令（我在名单上的第几格）
ll cnt[maxn];               // 我的手下人数（含我自己）
ll ord_[maxn];              // 名单上第 i 格是谁
ll timer_ = 0;

void dfs(ll u) {
    // 树的 dfs 不需要写"到边界就 return"：叶子没有手下，下面的 for 自然空转
    pos_[u] = ++timer_;     // 我是第 timer_ 个收到的
    ord_[timer_] = u;       // 名单第 timer_ 格写上我
    cnt[u] = 1;             // 手下人数：先算我自己
    for (ll v : ch[u]) {    // 按名单顺序，挨个把手下的活派下去
        dfs(v);
        cnt[u] += cnt[v];   // 手下 v 传完回来，把他那边的总人数加进来
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> q;
    for (ll i = 2; i <= n; ++i) {      // 1-based
        cin >> par_[i];
        ch[par_[i]].push_back(i);      // i 从小到大读入 ⇒ 天然就是名单顺序
    }

    dfs(1);

    for (ll i = 0; i < q; ++i) {
        ll u, k;
        cin >> u >> k;
        if (k > cnt[u]) cout << -1 << '\n';            // 手下不够 k 个
        else cout << ord_[pos_[u] + k - 1] << '\n';    // 从他那一格往后数
    }
    return 0;
}

// ⚠️ 为什么本地编译要加 -Wl,--stack,268435456：
//   如果树退化成一条链 1-2-3-…-200000，dfs 会递归 20 万层。
//   本机实测（Windows 默认栈 1MB）：n=20000 通过、n=25000 崩（0xC00000FD 栈溢出）。
//   评测机（Linux）栈大得多，递归通常不崩 —— 所以会出现"本机崩、交上去过"。
//   不加这个参数的话，你可以先用 cf1006e_big_chain.txt 复现这个崩溃。
