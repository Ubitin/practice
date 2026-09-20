// min 与 max 函数详解 —— 全部结论已用 g++ C++17 实测
#include <bits/stdc++.h>
using namespace std;

int main() {
    // ========== 1. 基本用法：两参数版本 ==========
    cout << "=== 1. 两参数 ===\n";
    int a = 7, b = 2;
    cout << min(a, b) << " " << max(a, b) << "\n";   // 2 7
    cout << min(3, 5) << " " << max(3, 5) << "\n";   // 3 5
    // 注意：只比较，不修改原值

    // ========== 2. 支持的类型 ==========
    cout << "=== 2. 各类型 ===\n";
    cout << min(3.14, 2.71) << "\n";                    // double
    cout << min(string("abc"), string("abd")) << "\n";  // string 按字典序 -> abc
    pair<int, int> p1 = {1, 9}, p2 = {2, 3};
    auto pm = min(p1, p2);   // pair 自带字典序 -> (1,9)
    cout << pm.first << "," << pm.second << "\n";

    // ========== 3. 三个及以上：initializer_list（花括号）==========
    cout << "=== 3. 多参数 ===\n";
    cout << min({5, 3, 8, 1}) << "\n";   // 1
    cout << max({5, 3, 8, 1}) << "\n";   // 8

    // ========== 4. 自定义比较器 ==========
    cout << "=== 4. 自定义比较 ===\n";
    // 比 second（pair 默认先比 first）
    auto cmpBySec = [](const pair<int,int>& x, const pair<int,int>& y) {
        return x.second < y.second;
    };
    auto pm2 = min(p1, p2, cmpBySec);   // (2,3) 的 second 更小
    cout << pm2.first << "," << pm2.second << "\n";

    // ========== 5. 数组/容器的最值：min_element / max_element ==========
    cout << "=== 5. 区间最值 ===\n";
    vector<int> v = {4, 1, 9, 3};
    cout << *min_element(v.begin(), v.end()) << "\n";   // 1（注意解引用！）
    cout << *max_element(v.begin(), v.end()) << "\n";   // 9
    int arr[4] = {4, 1, 9, 3};
    cout << *min_element(arr, arr + 4) << "\n";         // C 数组也行

    // ========== 6. 夹取 clamp：把数限制在区间内 ==========
    cout << "=== 6. clamp ===\n";
    int x = 15;
    int clamped = min(max(x, 0), 10);    // 夹到 [0, 10]
    cout << clamped << "\n";             // 10

    return 0;
}
