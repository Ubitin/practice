# min 与 max 函数详解：找最值的两兄弟

> 前置：STL 讲义 §3（算法库）、`fill函数详解.md`（同为 `<algorithm>` 的工具）。`min/max` 来自 `<algorithm>`（`<bits/stdc++.h>` 已含）。
> 演示程序：`test_\28.cpp`（全部结论已用 g++ C++17 实测）。

---

## 0. 一句话本质

**`min(a, b)` 返回两个值里较小的那个，`max(a, b)` 返回较大的那个。** 它们只做"比较 + 返回"，**不改动任何原值**。与之配套的 `min_element / max_element` 则是扫一整个区间找最值。

## 1. 两参数版：最基础形态（实测）

```cpp
int a = 7, b = 2;
min(a, b);   // 2
max(a, b);   // 7
min(3, 5);   // 3（直接传字面量也行）
max(3, 5);   // 5
```

**三个细节：**
1. 返回的是**两者的引用**，不产生新拷贝、不修改原值（`a` 还是 7，`b` 还是 2）；
2. 需要 `#include <algorithm>`（写了万能头就不用管）；
3. 参数**类型必须一致**：`min(3, 4.5)` 编译报错（int vs double）。解法是显式指定类型：`min<double>(3, 4.5)` → `3.0`。

## 2. 什么类型都能比（实测）

`min/max` 依赖 `<` 运算符，所以**能比较的类型都能用**：

```cpp
min(3.14, 2.71);                 // double：2.71
min(string("abc"), string("abd"));   // string 按字典序：abc
pair<int,int> p1 = {1, 9}, p2 = {2, 3};
min(p1, p2);                     // pair 自带字典序：先比 first → (1,9)
```

> 记忆钩子：**凡是用 `sort` 能排的，`min/max` 就能比**——因为它们共用同一套 `<` 规则。

## 3. 三个及以上：花括号版（initializer_list）

`min/max` 只收**两个**参数，想比多个就套花括号（C++11 起）：

```cpp
min({5, 3, 8, 1});   // 1
max({5, 3, 8, 1});   // 8
min({3, 1, 4, 1, 5});   // 1
```

> 内部原理：`{}` 把一串值打包成 `initializer_list`（初始化列表），函数在里面逐个比较。**机试记结论即可：括号里想放几个放几个。**

## 4. 自定义比较器（第三参数）

默认按 `<`（升序意义下取小/大）。想按自己的规则比，传第三个参数（函数/函数对象/lambda）：

```cpp
pair<int,int> p1 = {1, 9}, p2 = {2, 3};
// 默认：先比 first → min 是 (1,9)

// 想按 second 比：
auto cmpBySec = [](const pair<int,int>& x, const pair<int,int>& y) {
    return x.second < y.second;
};
min(p1, p2, cmpBySec);   // (2,3) 的 second=3 更小
```

> 比较器写法与 `sort` 的 cmp 完全一样：**返回 `true` 表示"第一个参数该排在前面/更小"**。`min_element` 的第三参数同理。

## 5. 区间最值：min_element / max_element（扫容器找最值）

两个参数的 `min/max` 只比**两个值**；要在一**整段区间**（数组、vector）里找最值，用这俩——**返回的是迭代器，别忘了 `*` 解引用**：

```cpp
vector<int> v = {4, 1, 9, 3};
*min_element(v.begin(), v.end());   // 1
*max_element(v.begin(), v.end());   // 9

int arr[4] = {4, 1, 9, 3};
*min_element(arr, arr + 4);         // C 数组也能用（区间 [arr, arr+4)）
```

**拿到的是"位置"不只是"值"**——常用它反推下标：

```cpp
auto it = min_element(v.begin(), v.end());
int pos = it - v.begin();       // 最小值的下标
```

三个坑：
1. **返回迭代器，必须解引用**：`min_element(...)` 直接 cout 会输出地址之类，`*` 才是值；
2. **空区间是未定义行为**：容器为空时 `min_element` 返回 `end()`，解引用就崩——先判 `!v.empty()`；
3. 区间规则和其他算法一致：**左闭右开 `[begin, end)`**。

## 6. 最常用套路：夹取 clamp（把数限制在范围内）

竞赛和课程里最常见的场景：**把分数/索引/输入"夹"在 `[lo, hi]` 区间内**，超过上限压到 hi、低于下限抬到 lo：

```cpp
int x = 15;
int clamped = min(max(x, 0), 10);    // 夹到 [0, 10]：15 → 10
// x = -3  →  max(-3,0)=0 → min(0,10)=0
// x = 5   →  max(5,0)=5  → min(5,10)=5
```

> 拆解：`max(x, lo)` 先保证**不低于下限**，外面的 `min(..., hi)` 再保证**不超过上限**。记忆：**先 max 兜底、再 min 封顶**。

## 7. 常见坑总结

1. **类型必须一致**：`min(3, 4.5)` 编译错 → `min<double>(3, 4.5)` 或统一类型；
2. **别拿返回值当"副本"改**：`min(a,b)` 返回引用，`min(a,b) = 100` 会真的改掉 a/b 里的一个（没人这么写，但要知道它返回的是引用）；
3. **min_element 空容器**：先判空再解引用；
4. **与 `#define min` 宏的冲突**：有些旧代码/库（如 `windows.h`）用 `#define min(a,b)` 宏，会覆盖 `std::min`——报错时查查是不是被宏劫持了（竞赛环境一般没有，知道即可）；
5. **相等时返回哪一个**：两值相等时，标准不保证返回哪一个（实现相关）——**需要"相等时的行为"就别依赖 min/max**。

## 8. 习题（答案已实测，先做再看）

**E1** 写出输出：
```cpp
cout << min(3, 8) << " " << max(3, 8) << "\n";
cout << min({7, 2, 9, 1, 5}) << "\n";
```

**E2** 补全：求 `vector<int> v` 中最大值的下标：
```cpp
auto it = ____(v.begin(), v.end());
int pos = ____;
```

**E3** 把分数 `score` 限制在 `[0, 100]`，越界自动截断，写出表达式：
```cpp
int safe = ________________;
```

**E4** 判断对错：
```cpp
A. min(a, b) 会修改 a、b 中较小的那个
B. min(3, 4.5) 可以直接编译通过
C. min_element(v.begin(), v.end()) 可以直接 cout 出最小值
D. min({3, 1, 4}) 可以求出 1
```

**E5** 读程序写输出：
```cpp
vector<int> v = {5, 2, 8, 2, 1};
auto it = min_element(v.begin(), v.end());
cout << *it << " " << (it - v.begin()) << "\n";
cout << min(max(100, 0), 60) << "\n";
```

---

## 答案与解析

**E1** → `3 8` 和 `1`
两参数比 3 和 8；花括号版在 {7,2,9,1,5} 里取最小 1。

**E2** → `auto it = max_element(v.begin(), v.end()); int pos = it - v.begin();`
`max_element` 返回指向最大值的迭代器，减 `begin()` 得下标。

**E3** → `min(max(score, 0), 100)`
先 `max(score, 0)` 保证 ≥0，再 `min(..., 100)` 保证 ≤100。

**E4** → **A、B、C 错；D 对**。
A 错：min 只返回引用，不改原值；B 错：int 与 double 混用编译错；C 错：要 `*min_element(...)` 解引用；D 对：花括号版支持多个参数。

**E5** → `1 4` 和 `60`
最小元素是 1，出现在下标 4（min_element 返回**第一个**最小值的迭代器）；`min(max(100,0),60)`：max(100,0)=100，min(100,60)=60。

---

**一句话总结**：`min/max` = 两个值取小/大（类型要一致、返回引用）；比多个用花括号 `min({...})`；扫整段用 `min_element/max_element`（返回迭代器，记得 `*` 和判空）；三者都能加自定义比较器；**夹取区间记口诀"先 max 兜底、再 min 封顶"**。
