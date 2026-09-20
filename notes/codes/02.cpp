#include <bits/stdc++.h>
using namespace std;

void show(const char* tag, const vector<int>& v) {
    cout << tag;
    for (int x : v) cout << x << " ";
    cout << "| size=" << v.size() << " cap=" << v.capacity() << "\n";
}

int main() {
    vector<int> v = {1, 2, 3, 4, 5};
    show("初始      : ", v);

    // ===== 末尾添加多个 =====
    v.insert(v.end(), 2, 99);              // 加 2 个 99
    show("insert 2*99: ", v);              // 1 2 3 4 5 99 99

    v.insert(v.end(), {10, 20, 30});       // 加一串不同的值（C++11 初值表）
    show("insert list: ", v);              // 1 2 3 4 5 99 99 10 20 30

    int arr[] = {7, 8};
    v.insert(v.end(), arr, arr + 2);       // 加数组区间 [arr, arr+2)
    show("insert 区间 : ", v);             // 1 2 3 4 5 99 99 10 20 30 7 8

    // ===== 末尾删除多个 =====
    v.resize(v.size() - 3);                // 减到只剩 size-3 个（去掉 7 8 20? 最后3个是 7 8 30? 看输出）
    show("resize -3  : ", v);              // 末尾 3 个被丢掉

    v.erase(v.end() - 2, v.end());         // 删掉最后 2 个
    show("erase -2   : ", v);

    v.pop_back();                          // 删 1 个
    show("pop_back   : ", v);

    v.clear();                             // 全删
    show("clear      : ", v);

    // 补充：resize 变大时补默认值 0
    v.resize(3);
    show("resize 3   : ", v);              // 0 0 0
    return 0;
}
