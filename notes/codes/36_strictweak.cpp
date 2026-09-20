// ============================================================
//  test_/36_strictweak.cpp —— 比较器不满足【严格弱序】的后果
//
//  单独成一个文件，因为它很可能直接崩溃（std::sort 越界访问）。
//  配套讲义：讲义/lambda函数详解.md
//
//  std::sort 要求比较器 comp 满足严格弱序（strict weak ordering），核心是：
//      comp(a, a) 必须为 false      （不可自反）
//      comp(a, b) 与 comp(b, a) 不能同时为 true
//  写成 <= 或 >= 就违反了这两条 → 未定义行为。
// ============================================================
#include <bits/stdc++.h>
using namespace std;

int main() {
    // 环境变量控制：MODE 0=正确的 <，1=错误的 <=；N 规模；RANGE 值域（越小重复越多）
    int mode = getenv("MODE") ? atoi(getenv("MODE")) : 0;
    int N    = getenv("N")    ? atoi(getenv("N"))    : 200000;
    int RANGE= getenv("RANGE")? atoi(getenv("RANGE")): 1000000;

    mt19937 rng(12345);
    vector<int> v(N);
    for (auto& x : v) x = (int)(rng() % RANGE);

    printf("mode=%d (%-8s) N=%-8d range=%-8d ", mode, mode ? "错误的<=" : "正确的<", N, RANGE);
    fflush(stdout);

    if (mode == 0) sort(v.begin(), v.end(), [](int a, int b) { return a < b; });
    else           sort(v.begin(), v.end(), [](int a, int b) { return a <= b; });

    bool ok = true;
    for (int i = 1; i < N; ++i) if (v[i - 1] > v[i]) { ok = false; break; }
    printf("→ sort 返回，结果%s\n", ok ? "有序（本次未暴露）" : "【已乱序】");
    fflush(stdout);
    return 0;
}
