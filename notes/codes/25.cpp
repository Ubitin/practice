#include <bits/stdc++.h>
using namespace std;

int main() {
    // ===== 1. 你的两行代码拆解 =====
    // auto k_plus = [](int n) { return (RAND_MAX + 1) / n * n; };
    // ① auto：让编译器推断类型
    // ② [](int n){...}：lambda 表达式 = 匿名函数
    //    [] 捕获列表（空=不捕获外部变量） (int n) 参数 { } 函数体
    auto k_plus = [](int n) { return (RAND_MAX + 1) / n * n; };
    cout << "k_plus(100) = " << k_plus(100) << "\n";   // 像普通函数一样调用

    // ===== 2. 捕获列表：lambda 的独门绝技（能"看见"外面的变量）=====
    int factor = 10;
    auto mul = [factor](int x) { return x * factor; };  // [factor] 捕获 factor 的副本
    cout << "捕获副本: mul(3) = " << mul(3) << "\n";
    auto add_ref = [&factor](int x) { return x + factor; };  // [&factor] 捕获引用
    factor = 100;                       // 引用捕获会看到最新值
    cout << "引用捕获: add_ref(1) = " << add_ref(1) << " (看到 100)\n";
    auto add_all = [=](int x) { return x + factor; };  // [=] 全部按值捕获
    auto add_all_ref = [&](int x) { return x + factor; }; // [&] 全部按引用

    // ===== 3. 最常见的实战用法：当 sort 的比较器（pair 讲义见过）=====
    vector<pair<int, int>> pts = {{3, 9}, {1, 5}, {2, 8}, {1, 2}};
    sort(pts.begin(), pts.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second < b.second;
        return a.first < b.first;
    });
    cout << "lambda 排序: ";
    for (auto& p : pts) cout << "(" << p.first << "," << p.second << ") ";
    cout << "\n";

    // ===== 4. 返回类型自动推导（C++14+）=====
    auto max_lambda = [](int a, int b) { return a > b ? a : b; };  // 推导为 int
    cout << "max_lambda(3, 7) = " << max_lambda(3, 7) << "\n";

    // ===== 5. 为什么用 auto：lambda 的类型无法手写 =====
    // 每个 lambda 都是独一无二的"匿名类"对象，类型名编译器才认识，只能 auto
    auto f1 = [] { return 1; };
    auto f2 = [] { return 1; };
    cout << "两个看似相同的 lambda 类型不同: " << (typeid(f1) == typeid(f2)) << "\n";
    return 0;
}
