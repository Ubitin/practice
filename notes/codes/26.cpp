#include <bits/stdc++.h>
using namespace std;

void show(const char* tag, const vector<int>& v) {
    cout << tag;
    for (int x : v) cout << x << " ";
    cout << "\n";
}

int main() {
    // ===== 1. 标准三连：sort → unique → erase（vector 版）=====
    vector<int> v = {20, 40, 32, 67, 40, 20, 89, 300, 400, 15};
    sort(v.begin(), v.end());                            // ① 排序（unique 只去"相邻"重复）
    show("排序后     : ", v);
    auto newEnd = unique(v.begin(), v.end());            // ② 去重，把重复的"挪"到末尾
    show("unique 后  : ", v);                            // 注意：末尾还有垃圾，size 没变
    v.erase(newEnd, v.end());                            // ③ 砍掉尾巴
    show("erase 后   : ", v);
    cout << "个数 = " << v.size() << "\n\n";

    // ===== 2. 只想统计"不同元素个数"（不 erase）=====
    vector<int> w = {3, 1, 3, 2, 1, 2, 5};
    sort(w.begin(), w.end());
    int cnt = unique(w.begin(), w.end()) - w.begin();    // 新末尾 - 开头 = 不同元素个数
    cout << "不同元素个数 = " << cnt << " (不删，只数)\n\n";

    // ===== 3. C 数组版（课程/竞赛都常见）=====
    int a[] = {7, 2, 7, 3, 2, 5, 7};
    int n = 7;
    sort(a, a + n);
    n = unique(a, a + n) - a;                            // 新末尾 - 开头，直接改 n
    cout << "C 数组去重后: ";
    for (int i = 0; i < n; i++) cout << a[i] << " ";
    cout << "\n\n";

    // ===== 4. 铁律演示：不排序直接 unique 会怎样 =====
    vector<int> bad = {1, 2, 1, 3, 1};
    auto badEnd = unique(bad.begin(), bad.end());        // 没排序！
    cout << "不排序 unique 后: ";
    for (auto it = bad.begin(); it != badEnd; ++it) cout << *it << " ";
    cout << "  <- 只去相邻重复，1 还留着\n\n";

    // ===== 5. 与 set 对比（同功能两条路）=====
    vector<int> data = {20, 40, 32, 67, 40, 20, 89, 300, 400, 15};
    set<int> s(data.begin(), data.end());                // set：插入即排序去重
    cout << "set 版: ";
    for (int x : s) cout << x << " ";
    cout << " (自动有序去重, 但单次插入 O(log n))\n";
    return 0;
}
