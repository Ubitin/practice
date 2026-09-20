#include <bits/stdc++.h>
using namespace std;

int main() {
    // ===== 1. 演示取模偏差：rand() % 10000 =====
    // RAND_MAX+1 = 32768 = 3*10000 + 2768 → 余数 0~2767 出现 4 次，2768~9999 出现 3 次
    srand(42);
    const int N = 1000000;
    vector<int> cnt(10000, 0);
    for (int i = 0; i < N; i++) cnt[rand() % 10000]++;
    auto groupMean = [&](int lo, int hi) {
        long long s = 0;
        for (int i = lo; i <= hi; i++) s += cnt[i];
        return s * 1.0 / (hi - lo + 1);
    };
    cout << "取模法 1e6 次: 0~2767 组均值=" << fixed << setprecision(1) << groupMean(0, 2767)
         << "  2768~9999 组均值=" << groupMean(2768, 9999)
         << "  (理论 122.1 vs 91.6，偏差明显)\n";

    // ===== 2. 拒绝采样：消除偏差 =====
    // 思路：把 [0, RAND_MAX] 截断到 [0, k)，k 是 10000 的整数倍；拒绝 >= k 的采样
    const int RANGE = 10000;
    int k = (RAND_MAX + 1) / RANGE * RANGE;      // 32768 内最大的 10000 倍数 = 30000
    vector<int> cnt2(RANGE, 0);
    int rejected = 0;
    for (int i = 0; i < N; i++) {
        int x;
        do { x = rand(); rejected++; } while (x >= k);   // 拒绝 >= k 的，重新采样
        cnt2[x % RANGE]++;
    }
    auto groupMean2 = [&](int lo, int hi) {
        long long s = 0;
        for (int i = lo; i <= hi; i++) s += cnt2[i];
        return s * 1.0 / (hi - lo + 1);
    };
    cout << "拒绝采样 1e6 次: 0~2767 组均值=" << groupMean2(0, 2767)
         << "  2768~9999 组均值=" << groupMean2(2768, 9999)
         << "  (都≈100，偏差消除)\n";
    cout << "拒绝占比: " << (rejected - N) * 100.0 / N << "% (理论 N·p/(1-p)，p=2768/32768)\n";

    // ===== 3. 拒绝采样思想的应用：蒙特卡洛求 π =====
    // 在 [0,1]^2 里均匀撒点，落在单位圆内的比例 → π/4
    mt19937 rng(7);
    uniform_real_distribution<double> u(0.0, 1.0);
    long long total = 10000000, inside = 0;
    for (long long i = 0; i < total; i++) {
        double x = u(rng), y = u(rng);
        if (x * x + y * y <= 1.0) inside++;
    }
    cout << "蒙特卡洛求 π ≈ " << fixed << setprecision(6) << 4.0 * inside / total
         << " (真实 3.141593)\n";
    return 0;
}
