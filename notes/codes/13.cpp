#include <bits/stdc++.h>
using namespace std;

int main() {
    // ===== 1. 声明与初始化 =====
    array<int, 5> a = {10, 20, 30};   // 只给 3 个 → 剩余自动补 0
    cout << "a = ";
    for (int x : a) cout << x << " "; // 10 20 30 0 0
    cout << "\n";

    // ===== 2. operator[]：读写（返回引用）=====
    a[1] = 99;                        // 写
    cout << "a[1] = " << a[1] << "\n"; // 读
    a[4] = a[0] + a[2];               // 运算后写回
    cout << "a[4] = " << a[4] << " (40)\n";

    // ===== 3. 越界行为：operator[] 不检查！at() 才抛异常 =====
    try {
        cout << a.at(5) << "\n";      // 越界 → 抛 out_of_range
    } catch (const out_of_range& e) {
        cout << "at(5) 抛出异常: " << e.what() << "\n";
    }
    // a[5] 不报错但越界（UB）——不演示，记住别写

    // ===== 4. 大小与内存 =====
    cout << "size()=" << a.size() << " sizeof(a)=" << sizeof(a)
         << " (5*4=20 字节, 无 capacity 概念)\n";

    // ===== 5. STL 全家桶：sort / begin / end / == =====
    array<int, 5> b = {3, 1, 4, 1, 5};
    sort(b.begin(), b.end());         // 用迭代器，C 数组做不到
    cout << "排序后 b: ";
    for (int x : b) cout << x << " ";
    cout << "\n";
    array<int, 5> c = {1, 1, 3, 4, 5};
    cout << "b == c ? " << (b == c) << " (元素逐个比较)\n";

    // ===== 6. tuple 接口：get<N> =====
    array<int, 3> t = {7, 8, 9};
    cout << "get<0>=" << get<0>(t) << " get<2>=" << get<2>(t) << "\n";

    // ===== 7. 与 C 数组对比：可以整体传参（拷贝）=====
    auto copyA = a;                   // 整体拷贝（C 数组不行）
    copyA[0] = -1;
    cout << "copyA[0]=" << copyA[0] << " 原 a[0]=" << a[0] << " (互不影响)\n";
    return 0;
}
