#include <bits/stdc++.h>
using namespace std;

void show(const char* tag, const vector<int>& v) {
    cout << tag;
    for (int x : v) cout << x << " ";
    cout << "\n";
}

int main() {
    // ===== 1. 删"指定值"的第一个 =====
    vector<int> v1 = {1, 2, 3, 2, 4};
    auto it = find(v1.begin(), v1.end(), 2);   // 找到第一个 2 的位置
    if (it != v1.end()) v1.erase(it);          // 没找到才安全
    show("删第一个2  : ", v1);                 // 1 3 2 4

    // ===== 2. 删"指定值"的所有 =====
    vector<int> v2 = {1, 2, 3, 2, 4, 2};
    v2.erase(remove(v2.begin(), v2.end(), 2), v2.end());   // 删除-擦除惯用法
    show("删所有2    : ", v2);                 // 1 3 4

    // ===== 3. 按下标删（第 k 个元素）=====
    vector<int> v3 = {10, 20, 30, 40};
    v3.erase(v3.begin() + 1);                  // 删下标 1（即 20）
    show("删下标1    : ", v3);                 // 10 30 40

    // ===== 4. 按条件删（循环 + 返回值，你学过的 it = erase(it)）=====
    vector<int> v4 = {1, 2, 3, 4, 5, 6};
    for (auto it = v4.begin(); it != v4.end(); ) {
        if (*it % 2 == 0) it = v4.erase(it);   // 删偶数，erase 返回下一个
        else ++it;
    }
    show("删所有偶数  : ", v4);                // 1 3 5

    // ===== 5. 越界/失败演示（正常代码里别这么写）=====
    vector<int> v5 = {1, 2, 3};
    auto it5 = find(v5.begin(), v5.end(), 99); // 找不到 → 返回 end()
    if (it5 == v5.end()) cout << "找不到99，不删（end() 不能 erase！）\n";
    return 0;
}
