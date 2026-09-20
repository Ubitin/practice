#include <bits/stdc++.h>
using namespace std;

int linearLogRecur(int n) {
    if (n <= 1) return 1;
    int count = linearLogRecur(n / 2) + linearLogRecur(n / 2);  // 2 个子问题，规模各减半
    for (int i = 0; i < n; i++) count++;                        // 每层额外 O(n) 循环
    return count;
}

int main() {
    cout << "n    返回值    n*(log2(n)+1)    n*log2(n)+n\n";
    for (int n = 1; n <= 16; n *= 2) {
        int v = linearLogRecur(n);
        cout << n << "    " << v << "       " << (int)(n * (log2(n) + 1))
             << "              " << (int)(n * log2(n) + n) << "\n";
    }
    // 直观对比：n 翻倍时，返回值和运行规模的增长比例
    cout << "\nT(8)=" << linearLogRecur(8) << " T(16)=" << linearLogRecur(16)
         << "  -> 翻倍后约 2.27 倍（O(n log n) 特征）\n";
    return 0;
}
