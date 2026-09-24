// 四种写法对照实验（每个模式放在独立函数里，避免互相影响栈帧）
// 用法: vec_probe.exe <mode>
//   mode 0: 全局 vector<ll> ch[maxn];         —— 用户现在的写法
//   mode 1: 全局 vector<vector<ll>> ch;       —— 空 vector，不 resize 直接 ch[x]
//   mode 2: 局部（函数内）vector<ll> ch[maxn];
//   mode 3: 局部 vector<vector<ll>> ch(n+1);  —— 正确用法
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll maxn = 200005;

vector<ll> gArr[maxn];        // mode 0：全局"数组 + vector"
vector<vector<ll>> gVec;      // mode 1：全局"vector 套 vector"，默认是空的

int n = 5;
void buildTree(int x) { }     // 占位（各模式里内联写）

void runGlobalArr() {                          // mode 0
    for (int i = 2; i <= n; ++i) gArr[i / 2].push_back(i);
    printf("node1 的儿子: ");
    for (ll v : gArr[1]) printf("%lld ", v);
    printf("\n");
}

void runGlobalVecEmpty() {                     // mode 1
    printf("gVec.size() = %zu  ← 里面一个元素都没有\n", gVec.size());
    fflush(stdout);
    for (int i = 2; i <= n; ++i) gVec[i / 2].push_back(i);   // ★ 越界访问空 vector
    printf("node1 的儿子: ");
    for (ll v : gVec[1]) printf("%lld ", v);
    printf("\n");
}

void runLocalArr() {                           // mode 2
    vector<ll> lArr[maxn];                     // ★ 4.8MB 全在【栈】上
    for (int i = 2; i <= n; ++i) lArr[i / 2].push_back(i);
    printf("node1 的儿子: ");
    for (ll v : lArr[1]) printf("%lld ", v);
    printf("\n");
}

void runLocalVec(int n_) {                     // mode 3
    vector<vector<ll>> ch(n_ + 1);             // 内容在【堆】上，栈上只有 24 字节
    for (int i = 2; i <= n_; ++i) ch[i / 2].push_back(i);
    printf("node1 的儿子: ");
    for (ll v : ch[1]) printf("%lld ", v);
    printf("\n");
}

int main(int argc, char** argv) {
    int mode = argc > 1 ? atoi(argv[1]) : 0;
    printf("sizeof(vector<ll>) = %zu 字节；maxn = %lld ⇒ 数组本体 = %.2f MB\n",
           sizeof(vector<ll>), maxn,
           maxn * (double)sizeof(vector<ll>) / 1048576.0);
    fflush(stdout);
    switch (mode) {
        case 0: runGlobalArr(); break;
        case 1: runGlobalVecEmpty(); break;
        case 2: runLocalArr(); break;
        default: runLocalVec(n); break;
    }
    return 0;
}
