#include <bits/stdc++.h>
using namespace std;
int main() {
    // 验证1: 指定大小 + 列表初始化，剩余元素是否自动补 0
    char c[3] = {'h', 'i'};
    printf("c[2] = %d (应为0), strlen(c) = %zu (应为2)\n", c[2], strlen(c));

    // 验证2: 不指定大小 + 列表初始化 → 数组大小=初始化器个数，无 '\0'
    char d[] = {'h', 'i'};
    printf("sizeof(d) = %zu (应为2), 没有\\0结尾\n", sizeof(d));
    // strlen(d) 是未定义行为，这里不打印

    // 验证3: 运行时赋值未初始化数组 → 其余元素是垃圾
    char e[5];
    e[0] = 'h'; e[1] = 'i';
    printf("e[2] = %d (垃圾值, 不确定), strlen(e) 未定义行为\n", e[2]);

    // 验证4: 字符串字面量超过数组大小 → 编译错误（取消注释试试）
    // char f[2] = "hi";   // initializer-string for 'char [2]' is too long
    return 0;
}
