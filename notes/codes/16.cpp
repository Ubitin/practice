#include <bits/stdc++.h>
using namespace std;

int main() {
    // ===== 1. 单次删中间：erase(begin()+k)，后面元素整体左移 =====
    vector<int> v = {1, 2, 3, 4, 5, 6};
    v.erase(v.begin() + 2);              // 删下标 2（值 3）
    cout << "删中间后: ";
    for (int x : v) cout << x << " ";    // 1 2 4 5 6
    cout << "  <- 4 5 6 全部左移了一位（O(n)）\n";

    // ===== 2. 多次删中间：每次 erase 都是 O(n) → 总共 O(n²) =====
    vector<int> w;
    for (int i = 1; i <= 100000; i++) w.push_back(i);

    auto t0 = chrono::steady_clock::now();
    for (int k = 0; k < 5000; k++) w.erase(w.begin() + 100);   // 连续删 5000 次中间
    auto t1 = chrono::steady_clock::now();
    cout << "连续 erase 删 5000 次: "
         << chrono::duration_cast<chrono::milliseconds>(t1 - t0).count() << " ms\n";

    // ===== 3. 正确姿势：标记法（一遍过滤，O(n)）=====
    vector<int> w2;
    for (int i = 1; i <= 100000; i++) w2.push_back(i);
    vector<bool> del(w2.size(), false);
    for (int k = 0; k < 5000; k++) del[100 + k] = true;        // 标记要删的（先算好位置）

    auto t2 = chrono::steady_clock::now();
    vector<int> keep;
    keep.reserve(w2.size());
    for (size_t i = 0; i < w2.size(); i++)
        if (!del[i]) keep.push_back(w2[i]);                    // 只拷贝没被标记的
    auto t3 = chrono::steady_clock::now();
    cout << "标记法 删 5000 个   : "
         << chrono::duration_cast<chrono::milliseconds>(t3 - t2).count() << " ms\n";
    cout << "keep.size() = " << keep.size() << " (100000 - 5000)\n";
    return 0;
}
