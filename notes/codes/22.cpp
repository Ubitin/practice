#include <bits/stdc++.h>
using namespace std;

int main() {
    // ===== 1. 老式：rand() 不播种 → 每次运行结果都一样 =====
    cout << "rand() 不播种（连跑两次程序对比）: ";
    for (int i = 0; i < 5; i++) cout << rand() % 100 << " ";
    cout << "\n";
    // 第二次运行这个程序会输出完全相同的 5 个数（因为没有 srand）

    // ===== 2. rand() + srand(time(0)) 播种 =====
    srand(time(0));                     // 以当前秒数做种子
    cout << "srand(time(0)) 后: ";
    for (int i = 0; i < 5; i++) cout << rand() % 100 << " ";   // %100 得 0~99
    cout << "\n";
    cout << "RAND_MAX = " << RAND_MAX << " (rand 最大值，只有 32767)\n\n";

    // ===== 3. 现代 C++11：mt19937 + uniform_int_distribution =====
    // 固定种子 → 可复现（对拍、调试需要）
    mt19937 rng(42);                    // 梅森旋转引擎，种子 42
    uniform_int_distribution<int> dis1(1, 6);   // 均匀整数分布 [1,6]
    cout << "mt19937(42) 固定种子 6 次掷骰: ";
    for (int i = 0; i < 6; i++) cout << dis1(rng) << " ";
    cout << "\n";

    // 随机种子（random_device 真随机取种子）
    mt19937 rng2(random_device{}());
    uniform_int_distribution<int> dis2(10, 99);   // [10, 99]
    cout << "随机种子 [10,99]: ";
    for (int i = 0; i < 8; i++) cout << dis2(rng2) << " ";
    cout << "\n";

    // 浮点均匀分布
    mt19937 rng3(7);
    uniform_real_distribution<double> dis3(0.0, 1.0);   // [0,1) 浮点
    cout << "浮点 [0,1): ";
    for (int i = 0; i < 5; i++) cout << fixed << setprecision(4) << dis3(rng3) << " ";
    cout << "\n\n";

    // ===== 4. 竞赛场景：生成对拍测试数据 =====
    mt19937 rng4(2024);
    uniform_int_distribution<int> n_dis(1, 1000);       // n 的范围
    uniform_int_distribution<int> v_dis(-1e9, 1e9);     // 值的范围
    cout << "对拍数据样例（n 和 n 个数）:\n";
    int n = n_dis(rng4);
    cout << n << "\n";
    for (int i = 0; i < n; i++) cout << v_dis(rng4) << " ";
    cout << "\n\n";

    // ===== 5. 随机打乱：shuffle（C++17）=====
    vector<int> cards = {1, 2, 3, 4, 5, 6, 7};
    shuffle(cards.begin(), cards.end(), mt19937(5));
    cout << "shuffle 洗牌: ";
    for (int x : cards) cout << x << " ";
    cout << "\n";
    return 0;
}
