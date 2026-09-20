#include <bits/stdc++.h>
using namespace std;

int main(){
    cout << "=== A. 用 cout 打印 char/signed char 的极值（会当字符输出！）===\n";
    cout << "  cout << numeric_limits<char>::max()          -> [" << numeric_limits<char>::max() << "]\n";
    cout << "  cout << numeric_limits<unsigned char>::max() -> [" << numeric_limits<unsigned char>::max() << "]\n";
    cout << "  (int) 强转后才是数字: " << (int)numeric_limits<char>::max()
         << " / " << (int)numeric_limits<unsigned char>::max() << "\n";

    cout << "\n=== B. printf 占位符与类型不匹配（-Wall 会有警告，但能编译）===\n";
    printf("  printf(\"%%d\",  (long long)LLONG_MAX)  -> %d   ← 只取低 32 位\n", (int)(long long)numeric_limits<long long>::max());
    printf("  printf(\"%%lld\", INT_MAX)  -> %lld   ← 按 64 位解释 32 位参数（垃圾值）\n",
           (long long)numeric_limits<int>::max());
    printf("  正确写法: printf(\"%%lld\", LLONG_MAX) -> %lld\n", LLONG_MAX);
    printf("  正确写法: printf(\"%%d\", INT_MAX)    -> %d\n", INT_MAX);

    cout << "\n=== C. 浮点 min() 被当成\"最小值\"用 ===\n";
    cout << "  numeric_limits<double>::min()    = " << numeric_limits<double>::min() << "   ← 最小正数！\n";
    cout << "  numeric_limits<double>::lowest() = " << numeric_limits<double>::lowest() << "   ← 真正的最负值\n";

    cout << "\n=== D. 用 INT_MAX 当 INF 再相加（溢出）===\n";
    int inf = numeric_limits<int>::max();
    cout << "  INT_MAX + 1        = " << inf + 1 << "   ← 溢出成负数\n";
    cout << "  INT_MAX + INT_MAX  = " << inf + inf << "\n";
    int a[3]; memset(a, 0x3f, sizeof(a));
    cout << "  memset(0x3f) 元素 = " << a[0] << "，两倍 = " << 2 * a[0] << "  (安全 ✓)\n";
    int b[3]; memset(b, 0x7f, sizeof(b));
    cout << "  memset(0x7f) 元素 = " << b[0] << "，两倍 = " << 2 * b[0] << "  ← 溢出成负数 ✗（别用 0x7f）\n";

    cout << "\n=== E. 忘了括号 / 取错成员 ===\n";
    cout << "  正确: numeric_limits<int>::max() = " << numeric_limits<int>::max() << "\n";
    cout << "  INT_MAX 与它相等? " << (INT_MAX == numeric_limits<int>::max() ? "是" : "否") << "\n";
    cout << "  注意 min() 对无符号类型是 0: unsigned -> " << numeric_limits<unsigned>::min() << "\n";

    cout << "\n=== F. 用 to_string / stoi 等转换时的截断 ===\n";
    cout << "  to_string(LLONG_MAX) = " << to_string(numeric_limits<long long>::max()) << "\n";
    cout << "  (int)stoll(\"" << to_string(LLONG_MAX) << "\") 会溢出: " << (int)stoll(to_string(LLONG_MAX)) << "\n";
    return 0;
}
