# fill 函数详解：array 与 C 数组的"一键填充"

> 前置：`array详解-operator下标访问.md`。`fill` 来自 `<algorithm>`（`<bits/stdc++.h>` 已含）。
> 演示程序：`test_\14.cpp`（全部结论已用 g++ C++17 实测）。

---

## 0. 一句话本质

**fill = 把一段区间（或整个数组）的所有元素统一赋成同一个值。** 对 `std::array` 有两种入口：

```cpp
a.fill(0);                            // ① 成员函数：整个 array 填 0（最简单）
fill(a.begin(), a.end(), 0);          // ② 算法 std::fill：填区间 [begin, end)
```

## 1. 两种"全填"方式（实测）

```cpp
array<int, 5> a = {1, 2, 3, 4, 5};
a.fill(0);                            // 成员版：a 变成 0 0 0 0 0

array<int, 5> b = {1, 2, 3, 4, 5};
fill(b.begin(), b.end(), 7);          // 算法版：b 变成 7 7 7 7 7
```

**区别**：`a.fill(v)` 只能"整个填"；`std::fill(first, last, v)` 是通用算法，能填任意区间（array、vector、C 数组都能用）。

## 2. 部分填充：只填一段（左闭右开，呼应你的迭代器知识）

```cpp
array<int, 5> c = {1, 2, 3, 4, 5};
fill(c.begin() + 1, c.begin() + 4, 9);   // 填下标 [1, 4) = 第 2~4 个
// c = 1 9 9 9 5   （实测）
```

> 区间规则和你学的 sort/二分一致：**包含 first，不包含 last**。`begin()+k` 指向下标 k。

## 3. fill_n：只填前 k 个

```cpp
array<int, 5> d = {1, 2, 3, 4, 5};
fill_n(d.begin(), 3, -1);              // 从 begin 开始连填 3 个
// d = -1 -1 -1 4 5   （实测）
```

## 4. 任意类型都能填（这是它比 memset 强的地方）

fill 用的是**赋值**语义，所以任何能赋值的类型都可以：

```cpp
array<string, 3> words = {"a", "b", "c"};
words.fill("hi");                      // hi hi hi（实测）

array<bool, 4> flags = {true, false, true, false};
fill(flags.begin(), flags.end(), true);// 1 1 1 1（实测）
```

## 5. 竞赛场景：多组数据的"清零重置"

机试常见的"多组输入、每组重新计数"套路：

```cpp
array<int, 10> cnt;
// ... 处理第一组数据，cnt 被填满 ...
cnt.fill(0);       // 一行重置所有计数（比 for 循环写，比 memset 通用）
```

## 6. 与 memset 的对比（C 课程衔接 + 注意事项）

```cpp
int ca[5];
fill(ca, ca + 5, 0);          // C 数组也能用 fill（算法版）
memset(ca, 0, sizeof(ca));    // C 风格：按字节填

// memset 填其他值要小心：
memset(ca, 1, sizeof(ca));    // ❌ 不是填 1！每个字节变 0x01010101 = 16843009
memset(ca, -1, sizeof(ca));   // ✅ 恰好全 0xFF = -1（补码），唯一安全的非零值
```

| | `fill` | `memset` |
|---|---|---|
| 原理 | 逐个赋值 | 按字节填 |
| 能填的值 | **任意值** | 0 和 -1（整型），其余是陷阱 |
| 适用类型 | 任意（string/pair 都行） | 只有整型/字符等"平凡类型" |
| 区间 | 任意子区间 | 只能从开头连续填 |
| 头文件 | `<algorithm>` | `<cstring>` |

> 结论：**C++ 里一律用 `fill`**；memset 只在需要极致速度且填 0/-1 时用。C 课程按教材学 memset 的概念，但要知道它的限制。

## 7. 常见坑

1. **区间写成"闭区间"**：`fill(begin(), begin()+5, v)` 填的是 0~4 共 5 个，不是 0~5——**左闭右开，last 要写成 begin()+个数**。
2. **fill_n 越界**：`fill_n(begin(), 10, v)` 而 array 只有 5 个 → 越界（UB）。个数必须 ≤ size()。
3. **memset 填 1 的陷阱**：以为是 1，实际是 16843009（每个字节 0x01）。
4. **`a.fill()` 和 `std::fill` 别混**：成员版只能整个填；想填部分用算法版。
5. **string 数组别用 memset**：会破坏 string 内部结构（它管理堆内存）——必须 fill。

## 8. 习题（答案已实测）

**E1** 写出输出：
```cpp
array<int, 5> a = {1, 2, 3, 4, 5};
a.fill(0); a[2] = 9;
for (int x : a) cout << x << " ";
```

**E2** 补全：把 `array<int, 6> a = {1,2,3,4,5,6}` 的**第 2~4 个**（下标 1~3）填成 0：
```cpp
fill(____, ____, 0);
```

**E3** 判断：`memset(arr, 1, sizeof(arr))` 把 int 数组每个元素变成多少？用 fill 怎么改？

**E4** 选择：想把 `array<string, 3>` 全部变成 "x"，用哪个？
```cpp
A. memset(a, 'x', sizeof(a));
B. a.fill("x");
C. fill(a.begin(), a.begin()+3, 'x');
D. a.assign(3, "x");   // 不存在
```

**E5** 读程序写输出：
```cpp
array<int, 4> a = {1, 2, 3, 4};
fill(a.begin() + 1, a.end(), 0);
for (int x : a) cout << x << " ";
```

---

## 答案与解析

**E1** → `0 0 9 0 0`
先全填 0，再把 a[2] 改成 9。

**E2** → `fill(a.begin() + 1, a.begin() + 4, 0);`
左闭右开：下标 [1, 4) 即第 2、3、4 个。

**E3** → 每个元素变 0x01010101 = **16843009**（不是 1！）。改法：`fill(arr, arr + n, 1);`

**E4** → **B**。A 的 memset 会破坏 string；C 的 `'x'` 是 char 不能赋给 string；D 不存在（那是 vector 的方法）。

**E5** → `1 0 0 0`
从下标 1 到末尾全填 0，a[0] 保留 1。

---

**一句话总结**：fill 是"区间统一赋值"——**成员版 `a.fill(v)` 整个填，算法版 `fill(first, last, v)` 填任意区间（左闭右开）**；任意类型可用、多组数据重置一行搞定；对比 memset 要记住"fill 填任意值、memset 只敢填 0/-1"。
