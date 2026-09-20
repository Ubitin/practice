#include <bits/stdc++.h>
using namespace std;

int main() {
    // ===== 1. C 风格：char[] 必须有 '\0' =====
    char cs[] = "hello";
    printf("char[]: sizeof=%zu, cs[5] = %d (应为0='\\0')\n", sizeof(cs), cs[5]);

    // ===== 2. C++ string：大小自己记，末尾也有 '\0'（C++11 起保证）=====
    string t = "hello";
    cout << "string: size()=" << t.size() << ", t[5] = " << (int)t[5]
         << " (应为0='\\0'，下标等于 size() 合法且是'\\0')\n";

    // ===== 3. 关键区别①：'\0' 可以藏在 string 内部（二进制数据）=====
    string bin("a\0b", 3);           // 显式构造：a、'\0'、b 三个字符
    cout << "bin.size() = " << bin.size() << " (3，含内部的'\\0')\n";
    cout << "bin[1] = " << (int)bin[1] << " (是'\\0')\n";
    // 用 cout << bin 会把 3 个字符都写出去（终端看不到 '\0' 而已）
    // 但若用 printf("%s", bin.c_str()) 会在第一个 '\0' 处停：
    printf("printf(%%s, bin.c_str()) = [%s]  <- 在内部'\\0'处截断！\n", bin.c_str());

    // ===== 4. 关键区别②：string 靠 size() 知道长度，不靠 '\0' =====
    string s2 = "hello";
    cout << "strlen(s2.c_str()) = " << strlen(s2.c_str())
         << " (能用，但绕路；正确姿势是 s2.size())\n";

    // ===== 5. char* 接收 string 的标准桥：c_str() =====
    string name = "Alice";
    printf("%s 长度 %zu\n", name.c_str(), name.size());
    return 0;
}
