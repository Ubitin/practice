#include <bits/stdc++.h>
using namespace std;

int main() {
    // 1. 字符字面量本身就是一个整数
    cout << "'A' 的值 = " << (int)'A' << "   'a' 的值 = " << (int)'a'
         << "   '0' 的值 = " << (int)'0' << "\n";   // 65 97 48

    // 2. char 参与运算 = 用 ASCII 数值算
    char c = 'A';
    cout << "c + 1 = " << c + 1 << "\n";            // 66（注意：结果是 int！）
    cout << "char(c + 1) = " << char(c + 1) << "\n"; // 'B'
    cout << "'A' + 3 = " << char('A' + 3) << "\n";   // 'D'

    // 3. 字符转数字：'9' - '0' = 9（竞赛最常用的套路）
    char d = '9';
    cout << "'9' - '0' = " << d - '0' << "\n";       // 9

    // 4. 大小写转换：相差 32（'a'-'A'=32）
    cout << "'a' - 'A' = " << 'a' - 'A' << "\n";     // 32
    cout << "大写化: " << char('b' - 32) << "  小写化: " << char('B' + 32) << "\n";
    cout << "异或32也能转: " << char('b' ^ 32) << "\n";  // 'B'（大小写切换技巧）

    // 5. 比较也是比 ASCII：字母序判断
    char ch = 'm';
    cout << "ch 是小写字母? " << (ch >= 'a' && ch <= 'z') << "\n";  // 1

    // 6. 整型提升：char + int 结果是 int
    cout << "sizeof(c + 1) = " << sizeof(c + 1) << " (int，4字节)  sizeof(c) = "
         << sizeof(c) << " (char，1字节)\n";

    // 7. 注意：char 有符号性（平台相关），但 ASCII 0~127 无影响
    cout << "char 最大值范围: " << (int)CHAR_MAX << "（ASCII 只需 0~127）\n";
    return 0;
}
