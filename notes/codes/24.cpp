#include <bits/stdc++.h>
using namespace std;

int main() {
    cout << "RAND_MAX = " << RAND_MAX << "\n";
    // rand() 能返回的所有值：0, 1, ..., RAND_MAX —— 闭区间
    // 值的"个数" = RAND_MAX - 0 + 1 = RAND_MAX + 1
    cout << "rand() 可能返回的值个数 = " << RAND_MAX + 1 << " (0~RAND_MAX 闭区间)\n\n";

    // 两种公式对比：k = (RAND_MAX+1)/n*n  vs  k = RAND_MAX/n*n
    auto k_plus = [](int n) { return (RAND_MAX + 1) / n * n; };
    auto k_no   = [](int n) { return RAND_MAX / n * n; };

    for (int n : {100, 10000, 16384, 32767, 32768}) {
        int a = k_plus(n), b = k_no(n);
        cout << "n=" << n << ":  (RAND_MAX+1)式 k=" << a
             << "  (RAND_MAX)式 k=" << b;
        if (a != b) cout << "  ← 有差别";
        if (b == 0 && n <= RAND_MAX + 1) cout << "  ← ②会无限循环！";
        cout << "\n";
    }

    cout << "\n直观解释：\n";
    cout << "n=16384: (RAND_MAX+1)式 k=32768 → 全接受，零拒绝；"
         << "(RAND_MAX)式 k=16384 → 白拒一半\n";
    cout << "n=32768: (RAND_MAX+1)式 k=32768 → 完美；"
         << "(RAND_MAX)式 k=0 → 全拒，死循环\n";
    return 0;
}
