#include <bits/stdc++.h>
using namespace std;

pair<int, int> makePoint(int x, int y) {   // 函数返回两个值
    return {x, y};
}

int main() {
    // 1. 声明与初始化（4 种方式）
    pair<int, int> p1 = {1, 2};             // C++11 花括号（最常用）
    pair<int, int> p2 = make_pair(3, 4);    // 旧写法
    pair<int, int> p3(5, 6);                // 构造函数
    cout << "p1 = (" << p1.first << "," << p1.second << ")\n";
    cout << "p2 = (" << p2.first << "," << p2.second << ")\n";

    // 2. first/second 访问与修改
    p1.first = 100;
    p1.second++;
    cout << "修改后 p1 = (" << p1.first << "," << p1.second << ")\n";

    // 3. 比较规则：先比 first，相同再比 second
    pair<int, int> a = {1, 2}, b = {1, 3}, c = {2, 1};
    cout << "比较: (1,2)<(1,3) = " << (a < b) << "  (first 相同比 second)\n";
    cout << "比较: (1,2)<(2,1) = " << (a < c) << "  (first 直接决定)\n";
    cout << "比较: (1,2)==(1,2) = " << (a == a) << "\n";

    // 4. 排序：sort 自动双关键字
    vector<pair<int, int>> v = {{3, 9}, {1, 5}, {2, 8}, {1, 2}};
    sort(v.begin(), v.end());
    cout << "排序后: ";
    for (auto& p : v) cout << "(" << p.first << "," << p.second << ") ";
    cout << "\n";

    // 5. 典型应用①：优先队列（默认按 first 大根堆）
    priority_queue<pair<int, int>> pq;
    pq.push({3, 9}); pq.push({1, 5}); pq.push({3, 2});
    cout << "pq.top() = (" << pq.top().first << "," << pq.top().second
         << ")   <- first 最大优先，first 同比较 second\n";

    // 6. 典型应用②：map 的二维键
    map<pair<int, int>, string> m;
    m[{2, 3}] = "点(2,3)";
    m[{1, 1}] = "原点附近";
    cout << "map<pair>: " << m[{2, 3}] << "\n";
    for (auto& kv : m) cout << "  (" << kv.first.first << "," << kv.first.second
                            << ") -> " << kv.second << "\n";

    // 7. 典型应用③：函数返回两个值 + 结构化绑定（C++17）
    auto pt = makePoint(7, 8);
    cout << "makePoint 返回 = (" << pt.first << "," << pt.second << ")\n";
    auto [x, y] = makePoint(10, 20);        // C++17 结构化绑定
    cout << "结构化绑定 x=" << x << " y=" << y << "\n";

    // 8. 邻接表：vector<pair<int,int>>（图论存边）
    vector<pair<int, int>> edges;
    edges.push_back({1, 2});
    edges.emplace_back(3, 4);               // 原地构造，略快
    for (auto& e : edges) cout << "边: " << e.first << "-" << e.second << "\n";
    return 0;
}
