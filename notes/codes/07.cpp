#include <bits/stdc++.h>
using namespace std;

int main() {
    // 1. size_t 是什么：无符号整数类型，表示"大小/个数"
    cout << "sizeof(size_t) = " << sizeof(size_t) << " 字节\n";
    cout << "size_t 最大值 = " << SIZE_MAX << "\n";          // 64 位机上 = 2^64-1

    // 2. 谁返回 size_t：v.size()、s.size()、strlen、sizeof、substr 参数……
    vector<int> v = {1, 2, 3};
    string s = "abc";
    cout << "v.size() 的类型是 size_t，s.size() 也是\n";
    cout << "sizeof(v.size()) = " << sizeof(v.size()) << "\n";

    // 3. 坑①：无符号回绕（size()-1 在空容器时是巨大数！）
    vector<int> empty;
    cout << "空 vector 的 size()-1 = " << (empty.size() - 1) << "\n";
    cout << "（不是 -1！0 减 1 回绕成 2^64-1）\n";

    // 4. 坑②：有符号与无符号比较（-1 会转成巨大无符号数）
    int a = -1;
    cout << "(-1 < 1u) = " << (a < 1u) << "   <- 输出 0（false），因为 -1 被转成 4294967295\n";

    // 5. 正确用法
    for (size_t i = 0; i < v.size(); i++) cout << v[i];       // 正序：直接用 size_t 下标
    cout << " | ";
    for (int i = (int)v.size() - 1; i >= 0; i--) cout << v[i]; // 倒序：必须强转 int，否则 i>=0 恒真死循环
    cout << "\n";

    // 6. 打印：printf 用 %zu；cout 直接输出
    printf("printf %%zu: %zu\n", v.size());
    cout << "cout: " << v.size() << "\n";
    return 0;
}
