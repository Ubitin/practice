#include <bits/stdc++.h>
using namespace std;

int main() {
    // ===== 场景A：reserve 后 push_back —— 迭代器有效 ✓ =====
    cout << "=== A. reserve(100) 后 push_back 50 次 ===\n";
    vector<int> v;
    v.reserve(100);
    v.push_back(1);
    auto itA = v.begin();          // 保存指向 v[0] 的迭代器
    int* pA = &v[0];
    for (int i = 2; i <= 50; i++) v.push_back(i);
    cout << "size=" << v.size() << " cap=" << v.capacity() << "\n";
    cout << "itA 指向的值: " << *itA << "  (应仍是 1)\n";
    cout << "v[0] 地址: " << pA << "  现在 &v[0] 的地址: " << &v[0] << "\n";
    cout << "两地址相同 = 内存没搬家 = 迭代器仍有效\n\n";

    // ===== 场景B：不 reserve，push_back 触发扩容 —— 迭代器失效 ✗ =====
    cout << "=== B. 不 reserve，push_back 触发扩容 ===\n";
    vector<int> w;
    w.push_back(1);
    int* pB = &w[0];
    int oldCap = w.capacity();
    for (int i = 2; i <= 100; i++) w.push_back(i);
    cout << "扩容前 &w[0]=" << pB << "  扩容后 &w[0]=" << &w[0] << "\n";
    cout << "地址变了 = 旧内存已释放 = 之前保存的指针/迭代器全部作废(悬垂)\n";
    cout << "此时解引用 pB 是未定义行为(读已释放内存)\n\n";

    // ===== 场景C：reserve 足够，但中间 insert —— 位移型失效 ✗ =====
    cout << "=== C. reserve(100) 后中间 insert（容量足够） ===\n";
    vector<int> u = {10, 20, 30, 40};
    u.reserve(100);                // 容量足够，绝不会扩容
    auto itC = u.begin() + 3;      // 指向 40（插入点之后）
    cout << "insert 前 *itC = " << *itC << " (指向 40)\n";
    u.insert(u.begin() + 1, 999);  // 在 20 前面插入
    cout << "insert 后 *itC = " << *itC << "  <- 现在指向的元素是? (位移发生)\n";
    cout << "插入点之后的元素整体右移，旧迭代器指向的位置被新元素顶替\n";
    cout << "u = ";
    for (int x : u) cout << x << " ";
    cout << "\n注意：标准规定此时 itC 已失效(解引用为未定义行为)，实测往往指向移位后的元素\n\n";

    // ===== 场景D：reserve 足够 + 末尾 insert —— 已有元素迭代器有效 ✓ =====
    cout << "=== D. reserve(100) 后末尾 insert ===\n";
    vector<int> t = {1, 2, 3};
    t.reserve(100);
    auto itD = t.begin();          // 指向 1
    t.insert(t.end(), {4, 5, 6});  // 末尾插入，不扩容
    cout << "*itD = " << *itD << " (仍有效)\n";
    cout << "注意：t.end() 这个迭代器本身失效了(末尾变了)，要重新取\n";

    return 0;
}
