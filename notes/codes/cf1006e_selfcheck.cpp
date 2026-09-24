// ⚠️ 这份是【自检/答案核对工具】，里面含一份参考实现 —— 想自己写就别打开这个文件。
//
// 自检目标：验证"一棵子树在【一次全局 DFS 的先根序】里占一段连续区间"这条结论。
//   A) 暴力：对每个 (u,k)，从 u 现场按孩子顺序 DFS，数到第 k 个
//   B) 预处理：全局一次 DFS 求 tin/sz/ord，答案 = ord[tin[u] + k - 1]（k > sz[u] 则 -1）
// 两法在大量随机树上逐对比较，必须完全一致。
#include <bits/stdc++.h>
using namespace std;

int n;
vector<int> ch[64];
int tin_[64], sz_[64], ord_[64], timer_ = 0;

void dfsPre(int u) {                 // B) 全局先根序
    tin_[u] = ++timer_;
    ord_[timer_] = u;
    sz_[u] = 1;
    for (int v : ch[u]) {
        dfsPre(v);
        sz_[u] += sz_[v];
    }
}

int cnt_, found_;
void dfsSim(int u, int k) {          // A) 现场模拟
    if (found_ != -1) return;
    if (++cnt_ == k) { found_ = u; return; }
    for (int v : ch[u]) {
        dfsSim(v, k);
        if (found_ != -1) return;
    }
}

int main() {
    mt19937_64 rng(20260922ULL);
    long long pairs = 0, bad = 0;
    string firstBad;
    for (int tc = 0; tc < 20000; ++tc) {
        n = 1 + (int)(rng() % 12);
        for (int i = 1; i <= n; ++i) ch[i].clear();
        for (int i = 2; i <= n; ++i) {
            int style = (int)(rng() % 3);
            int p = (style == 0) ? 1 : (style == 1 ? i - 1 : 1 + (int)(rng() % (i - 1)));
            ch[p].push_back(i);
        }
        timer_ = 0;
        dfsPre(1);
        for (int u = 1; u <= n; ++u)
            for (int k = 1; k <= n + 1; ++k) {
                cnt_ = 0;
                found_ = -1;
                dfsSim(u, k);
                int bruteAns = found_;                               // A
                int fastAns = (k <= sz_[u]) ? ord_[tin_[u] + k - 1] : -1;  // B
                ++pairs;
                if (bruteAns != fastAns) {
                    ++bad;
                    if (firstBad.empty()) {
                        firstBad = "n=" + to_string(n) + " u=" + to_string(u) +
                                   " k=" + to_string(k) + " 暴力=" + to_string(bruteAns) +
                                   " 预处理=" + to_string(fastAns);
                    }
                }
            }
    }
    printf("随机树 20000 棵，逐对比较 (u,k) 共 %lld 组\n", pairs);
    printf("  不一致 = %lld\n", bad);
    printf("  首个反例：%s\n", firstBad.empty() ? "（无）" : firstBad.c_str());
    printf("结论文：【子树 = 先根序中连续区间 [tin[u], tin[u]+sz[u]-1]】%s\n",
           bad == 0 ? "成立 ✅" : "被推翻 ✘");
    return 0;
}
