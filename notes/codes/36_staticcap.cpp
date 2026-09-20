// 实测：捕获 static 局部变量时，GCC 到底做了什么？
#include <bits/stdc++.h>
using namespace std;

int main() {
    static int sv = 7;
    auto f = [sv]() { return sv; };     // 语法上被 GCC 接受
    printf("定义后立刻调用 f() = %d\n", f());
    sv = 99;                             // 改动静态变量
    printf("把 sv 改成 99 后  f() = %d\n", f());
    printf("  若输出 7  → 真的拷了一份副本（值捕获语义）\n");
    printf("  若输出 99 → 只是直接访问静态变量，捕获被忽略了\n");

    auto g = []() { return sv; };        // 不捕获，直接访问
    printf("不捕获直接读 sv 的 lambda：%d（说明静态变量本来就能直接用）\n", g());

    // 引用捕获静态变量
    auto h = [&sv]() { return sv; };
    sv = 123;
    printf("引用捕获 [&sv] 后再改成 123：%d\n", h());
    return 0;
}
