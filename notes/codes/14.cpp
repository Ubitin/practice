#include <bits/stdc++.h>
using namespace std;

void show(const char* tag, const array<int, 5>& a) {
    cout << tag;
    for (int x : a) cout << x << " ";
    cout << "\n";
}

int main() {
    // ===== 1. 两种"全填"方式 =====
    array<int, 5> a = {1, 2, 3, 4, 5};
    a.fill(0);                            // ① 成员函数：整个数组填 0
    show("a.fill(0)   : ", a);            // 0 0 0 0 0

    array<int, 5> b = {1, 2, 3, 4, 5};
    fill(b.begin(), b.end(), 7);          // ② 算法 std::fill：区间 [begin,end) 填 7
    show("fill 区间    : ", b);           // 7 7 7 7 7

    // ===== 2. 部分填充：只填一段区间 [begin+1, begin+4) =====
    array<int, 5> c = {1, 2, 3, 4, 5};
    fill(c.begin() + 1, c.begin() + 4, 9);   // 填第 2~4 个（左闭右开）
    show("部分填充     : ", c);              // 1 9 9 9 5

    // ===== 3. fill_n：填前 k 个 =====
    array<int, 5> d = {1, 2, 3, 4, 5};
    fill_n(d.begin(), 3, -1);                 // 前 3 个填 -1
    show("fill_n 前3个 : ", d);               // -1 -1 -1 4 5

    // ===== 4. 其他类型也能填（赋值语义，不是 memset）=====
    array<string, 3> words = {"a", "b", "c"};
    words.fill("hi");
    cout << "fill string  : ";
    for (auto& w : words) cout << w << " ";   // hi hi hi
    cout << "\n";

    array<bool, 4> flags = {true, false, true, false};
    fill(flags.begin(), flags.end(), true);
    cout << "fill bool    : ";
    for (bool f : flags) cout << f << " ";    // 1 1 1 1
    cout << "\n";

    // ===== 5. 竞赛场景：多组数据的"清零重置" =====
    // 每次读入一组数据前，把计数数组重置
    array<int, 10> cnt;
    for (int i = 0; i < 10; i++) cnt[i] = i;
    // 新一组数据来了：
    cnt.fill(0);                              // 一行重置，比循环快写
    cout << "重置后 cnt  : ";
    for (int x : cnt) cout << x << " ";       // 0 0 0 0 0 0 0 0 0 0
    cout << "\n";

    // ===== 6. C 数组对照：fill vs memset =====
    int ca[5] = {1, 2, 3, 4, 5};
    fill(ca, ca + 5, 0);                      // C 数组也能用 fill
    cout << "C数组 fill  : ";
    for (int x : ca) cout << x << " ";        // 0 0 0 0 0
    cout << "\n";
    int cb[5];
    memset(cb, 0, sizeof(cb));                // memset 只能填 0 或 -1（按字节）
    cout << "C数组 memset: ";
    for (int x : cb) cout << x << " ";        // 0 0 0 0 0
    cout << "\n";
    return 0;
}
