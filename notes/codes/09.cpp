#include <bits/stdc++.h>
using namespace std;

void sep(const char* s) { cout << "== " << s << " ==\n"; }

int main() {
    sep("1. 默认精度：有效数字");
    cout << setprecision(3) << 123.456 << "\n";     // 3 位有效数字
    cout << setprecision(3) << 1234.56 << "\n";     // 大数变科学计数法?
    cout << setprecision(6) << 123.456789 << "\n";  // 默认 6 位
    cout << 123.456789 << "\n";                     // 完全默认

    sep("2. fixed + setprecision：小数位数");
    cout << fixed << setprecision(2) << 123.456 << "\n";   // 123.46
    cout << setprecision(2) << 3.14159 << "\n";            // 3.14
    cout << fixed << setprecision(0) << 3.7 << "\n";       // 4（四舍五入）

    sep("3. 科学计数法");
    cout << scientific << setprecision(3) << 123456.789 << "\n";  // 1.235e+05
    cout << fixed << setprecision(2) << 0.0001234 << "\n";       // 0.00? 看输出
    cout << defaultfloat << 0.0001234 << "\n";                   // 恢复默认

    sep("4. 宽度与填充");
    cout << setw(10) << 42 << "|\n";              // 右对齐 10 格
    cout << setw(10) << left << 42 << "|\n";      // 左对齐
    cout << right;                                // 恢复右对齐（left/right 是持久的！）
    cout << setfill('0') << setw(5) << 42 << "\n";// 00042
    cout << setfill('*') << setw(6) << 42 << "\n";// ****42
    cout << setfill(' ') << 42 << "\n";           // 重置填充
    cout << "setw 一次性:[" << setw(8) << 1 << "][" << 2 << "]\n";  // 第二个没有宽度

    sep("5. 时间补零（实战场景）");
    int h = 9, m = 5, s = 0;
    cout << right << setfill('0') << setw(2) << h << ":" << setw(2) << m << ":" << setw(2) << s << "\n";
    cout << setfill(' ');

    sep("6. 浮点精度陷阱");
    cout << fixed << setprecision(20) << 0.1 + 0.2 << "\n";
    cout << fixed << setprecision(2) << 0.1 + 0.2 << "\n";  // 0.30

    sep("7. printf 对照");
    printf("%5d|%-5d|%05d\n", 42, 42, 42);
    printf("%.2f|%8.2f|%.0f\n", 3.14159, 3.14159, 3.7);
    return 0;
}
