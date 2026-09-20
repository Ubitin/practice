#include <bits/stdc++.h>
using namespace std;

// 计时工具
double ms(chrono::steady_clock::time_point a, chrono::steady_clock::time_point b) {
    return chrono::duration_cast<chrono::microseconds>(b - a).count() / 1000.0;
}

int main() {
    // ===== 1. 语义陷阱：memset 的 n 是"字节数"，resize 的 n 是"元素个数" =====
    int arr[100] = {0};
    for (int i = 0; i < 100; i++) arr[i] = 1;
    memset(arr, 0, 100);              // ❌ 只清了 100 字节 = 25 个 int！
    int cleared = 0;
    for (int i = 0; i < 100; i++) if (arr[i] == 0) cleared++;
    cout << "memset(arr,0,100) 后清零的元素数 = " << cleared << "/100 (应写 n*sizeof(int))\n\n";

    // ===== 2. 复杂度对比：重置大数组为全 0（取 3 次最小，防优化）=====
    const int N = 50000000;                    // 5000 万个 int ≈ 200MB
    vector<int> v(N, 1);
    int* raw = new int[N];
    for (int i = 0; i < N; i++) raw[i] = 1;
    volatile long long sink = 0;

    auto bench = [&](auto f) {
        double best = 1e18;
        for (int r = 0; r < 3; r++) {
            auto t0 = chrono::steady_clock::now();
            f();
            auto t1 = chrono::steady_clock::now();
            best = min(best, ms(t0, t1));
        }
        return best;
    };

    double t_memset = bench([&]{ memset(raw, 0, N * sizeof(int)); sink += raw[0]; });
    double t_fill   = bench([&]{ fill(v.begin(), v.end(), 0);    sink += v[0]; });
    double t_assign = bench([&]{ v.assign(N, 0);                  sink += v[0]; });

    vector<int> big(N, 5);
    auto c0 = chrono::steady_clock::now();
    big.resize(0);
    auto c1 = chrono::steady_clock::now();
    sink += big.size();                                  // 让 resize(0) 生效不被优化
    auto r0 = chrono::steady_clock::now();
    big.resize(N, 0);
    auto r1 = chrono::steady_clock::now();
    sink += big[N - 1];                                  // 读最后一个，迫使填充真正执行
    double t_clear = ms(c0, c1);
    double t_refill = ms(r0, r1);

    cout << "重置 " << N << " 个 int (3 次取最小):\n";
    cout << "  memset(字节) : " << fixed << setprecision(2) << t_memset << " ms\n";
    cout << "  std::fill    : " << t_fill << " ms\n";
    cout << "  assign       : " << t_assign << " ms\n";
    cout << "  resize(0)    : " << t_clear << " ms  <- int 平凡析构, 只改 size\n";
    cout << "  resize(N)    : " << t_refill << " ms\n";
    cout << "sink = " << sink << "\n";

    // ===== 4. 结论验证：真正要"清零计数数组"时的推荐 =====
    cout << "\n清零计数数组推荐: memset(arr, 0, sizeof(arr)); 或 fill(v.begin(), v.end(), 0);\n";
    delete[] raw;
    return 0;
}
