# vector 删元素全攻略：单删/全删/中间删/循环删/标记法

> 前置：`vector迭代器失效与reserve.md`（erase 后迭代器失效）。所有结论已用 g++ C++17 实测（演示程序 `test_\15.cpp`、`test_\16.cpp`）。

---

## 0. 一张表选对写法（其余都是展开）

| 场景 | 写法 | 复杂度 |
|---|---|---|
| 删指定值的**第一个** | `find` + `erase` | O(n) |
| 删指定值的**所有** | `erase(remove(...), end())` | O(n) |
| 按**下标**删（含中间） | `erase(begin()+k)` | O(n) |
| **循环**按条件删 | `it = v.erase(it)` | O(n) |
| 多次删中间（性能版） | **标记法** | O(n) |

## 1. 删指定值的第一个

```cpp
auto it = find(v.begin(), v.end(), 2);   // 找到第一个 2 的位置
if (it != v.end()) v.erase(it);          // 找到才删
// 实测：{1,2,3,2,4} → {1,3,2,4}
```

## 2. 删指定值的所有（remove-erase 惯用法，背下来）

```cpp
v.erase(remove(v.begin(), v.end(), 2), v.end());   // 一行删光所有 2
// 实测：{1,2,3,2,4,2} → {1,3,4}
```
原理：`remove` 把所有 2 移到末尾并返回"新末尾"，`erase` 砍掉末尾那段——**"先挪后砍"**。

## 3. 按下标删（含删中间）

```cpp
v.erase(v.begin() + 1);    // 删下标 1
v.erase(v.begin() + k);    // 删任意下标 k（越界是 UB，先保证 k < size()）
// 实测：{10,20,30,40} 删下标 1 → {10,30,40}，后面整体左移
```

## 4. 循环按条件删（你学过的 `it = erase(it)`）

```cpp
for (auto it = v.begin(); it != v.end(); ) {
    if (*it % 2 == 0) it = v.erase(it);   // 删了就不手动 ++（erase 返回下一个）
    else ++it;
}
// 实测：{1,2,3,4,5,6} 删偶数 → {1,3,5}
```

## 5. 性能：为什么删中间是 O(n)，多次删怎么办

**单次删中间**：`erase` 会把后面的元素全部左移一位——这是"连续内存"的代价（随机访问 O(1) 换来中间插删 O(n)）。

**多次删中间**：每次 erase 都 O(n) → 总共 **O(n²)**。实测对比（10 万个元素删 5000 次中间）：

| 方法 | 实测耗时 |
|---|---|
| 循环 erase 5000 次 | **172 ms** |
| 标记法 | **≈ 0 ms** |

**标记法（竞赛推荐）**：先"记账"后"搬家"——
```cpp
vector<bool> del(v.size(), false);        // ① 先标记所有要删的位置
for (每个要删的下标 k) del[k] = true;
vector<int> keep;                          // ② 一遍过滤，只拷没标记的
for (size_t i = 0; i < v.size(); i++)
    if (!del[i]) keep.push_back(v[i]);
```

## 6. 选型：频繁删中间就别用 vector

| 操作特征 | 该用 |
|---|---|
| 只删末尾 | vector（pop_back O(1)） |
| 偶尔删中间 | vector + erase（O(n)，可接受） |
| **频繁删中间** | **标记法**（O(n)）或 `list` |
| 频繁删 + 常查值 | `set` / `multiset`（删 O(log n)） |

> 竞赛口诀：**"删中间很频繁" → 先想 set / 标记法；"删一两次" → erase 直接写。**

## 7. 三个必记的坑

1. **`end()` 不能 erase**：`find` 找不到返回 `end()`，`v.erase(end())` 是未定义行为——删前必判 `it != v.end()`。
2. **erase 后迭代器失效**：删除点及之后全失效（元素左移）——所以循环删除必须用返回值 `it = v.erase(it)`，绝不能写：
```cpp
for (auto it = v.begin(); it != v.end(); ++it)
    if (*it % 2 == 0) v.erase(it);    // ❌ 跳元素 + 越界
```
3. **别用"删除后下标"继续访问**：`v.erase(begin()+k)` 之后，原 k 后面的元素下标全部 -1，之前算好的下标作废。

## 8. 习题（答案均基于实测）

**E1** 写出输出：
```cpp
vector<int> v = {1, 2, 3, 4, 5};
v.erase(v.begin() + 1);
for (int x : v) cout << x << " ";
```

**E2** 写出输出：
```cpp
vector<int> v = {1, 3, 2, 3, 3, 4};
v.erase(remove(v.begin(), v.end(), 3), v.end());
for (int x : v) cout << x << " ";
```

**E3** 判断对错：
```cpp
A. v.erase(v.end()) 是合法操作
B. 删中间元素后，它后面的迭代器全部失效
C. vector 中间插入/删除都是 O(n)
D. remove(v.begin(), v.end(), x) 会真的删除元素
```

**E4** 补全：循环删除所有偶数，用返回值更新迭代器：
```cpp
for (auto it = v.begin(); it != v.end(); ) {
    if (*it % 2 == 0) ____ = v.erase(____);
    else ____;
}
```

**E5** 概念：10 万个元素里要删 5000 个中间位置的元素，用"每次 erase"和"标记法"分别是什么复杂度？竞赛里选哪个？

---

## 答案与解析

**E1** → `1 3 4 5`（删下标 1 的 2，后面左移）。

**E2** → `1 2 4`（remove-erase 删光所有 3）。

**E3** → **A 错、D 错；B、C 对**。
A：erase(end()) 是 UB；D：remove 只"挪"不"删"，必须配 erase 才真正删除（所以叫"惯用法"）。

**E4** →
```cpp
it = v.erase(it);   // 删了：用返回值更新，不手动 ++
++it;               // 没删：手动前进
```

**E5** → 每次 erase：O(n²)（5000 次 × 每次 O(n) 左移）；标记法：O(n)（一遍标记 + 一遍过滤）。竞赛选**标记法**（实测 172ms vs 0ms）。

---

**一句话总结**：删元素先对号入座——**删一个值**用 `find+erase`，**删所有值**用 `remove+erase`，**循环删**用 `it=erase(it)`，**删中间且次数多**用**标记法**；永远记住"erase 后迭代器失效、end() 不能 erase、vector 中间操作 O(n)"。
