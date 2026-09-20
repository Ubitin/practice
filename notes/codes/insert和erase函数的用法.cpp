#include <bits/stdc++.h>
using namespace std;

void show(const vector<int>& v) {
    for (int x : v) cout << x << " ";
    cout << "| size=" << v.size() << " cap=" << v.capacity() << "\n";
}

int main() {
    vector<int> v = {1, 2, 3, 4, 5};
    show(v);                                   // 1 2 3 4 5

    // ===== insert：在"某位置之前"插入，参数是迭代器 =====
    v.insert(v.begin(), 0);                    // 在最前面插入 → {0,1,2,3,4,5}
    show(v);

    // 在第 3 个元素（下标 2，值 2）之前插入 99
    auto it = v.insert(v.begin() + 2, 99);     // 返回指向新插入元素(99)的迭代器
    show(v);                                   // {0,1,99,2,3,4,5}
    cout << "insert 返回的迭代器指向: " << *it << "\n";   // 99

    // insert 的其它重载
    v.insert(v.begin() + 1, 3, 7);             // 下标 1 前插入 3 个 7
    show(v);                                   // {0,7,7,7,1,99,2,3,4,5}

    int arr[] = {50, 60};
    v.insert(v.end(), arr, arr + 2);           // 末尾插入一段区间（左闭右开）
    show(v);                                   // {0,7,7,7,1,99,2,3,4,5,50,60}

    // ===== erase：删除指定位置或区间，参数也是迭代器 =====
    v.erase(v.begin());                        // 删掉第一个元素
    show(v);                                   // {7,7,7,1,99,2,3,4,5,50,60}

    v.erase(v.begin() + 3, v.begin() + 6);     // 删掉 [下标3, 下标6) 共 3 个
    show(v);                                   // {7,7,7,99,3,4,5,50,60}

    // ===== erase 的返回值：边遍历边删除的正确姿势 =====
    vector<int> w = {1, 2, 3, 4, 5, 6};
    for (auto it = w.begin(); it != w.end(); ) {
        if (*it % 2 == 0)
            it = w.erase(it);   // 删除后返回"被删元素的下一个"迭代器
        else
            ++it;               // 不删才手动前进
    }
    show(w);                     // {1,3,5}

    return 0;
}