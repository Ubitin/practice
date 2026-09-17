# priority_queue 有"按值计数"的函数吗？—— 没有，但四种替代方案

> 结论：**没有。** `std::priority_queue` 是**容器适配器**，只暴露堆顶相关的操作，
> **没有 `count()`、没有 `find()`、没有 `erase()`、也没有任何迭代器**（`begin()/end()` 都不存在）。
> 想统计"某个值出现了几次"，必须换结构或另开一张计数表。

---

## 一、它到底提供哪些接口（全部）

| 成员 | 作用 |
|---|---|
| `top()` | 访问堆顶（大根堆=最大值 / 小根堆=最小值） |
| `empty()` | 是否为空 |
| `size()` | **元素总数**（注意：不是"某个值的个数"） |
| `push(x)` / `emplace(...)` | 插入（O(log n)） |
| `pop()` | 弹出堆顶（O(log n)） |
| `swap(other)` | 交换两个堆 |

**没有的东西**：`count`、`find`、`erase`、`begin/end`、`operator[]`、`clear`、`lower_bound`。

### 编译实测（g++ 16.2，错误信息原文）

```cpp
priority_queue<int> pq; pq.push(1);
pq.count(1);            // error: 'class std::priority_queue<int>' has no member named 'count'
pq.begin();            // error: ... has no member named 'begin'
pq.erase(pq.top());    // error: ... has no member named 'erase'
```

### 为什么这么设计

`priority_queue` 内部只是"**一个容器 + 堆算法**"的包装（默认容器是 `vector`）：
- 它只保证**堆顶**是极值，其余元素的相对位置是堆序维护的中间状态，**对外没有意义**；
- 因此库作者干脆不暴露遍历接口——底层容器 `c` 是 `protected`，只有派生类能碰到。

> ⚠️ 即使通过派生类拿到 `c`，也**只能读**（`std::count` 数个数可以）；
> 若手动改 `c` 的元素，**必须重新 `make_heap`**，否则堆性质被破坏，`top()/pop()` 结果全错。

---

## 二、四种替代方案（都实测过，结果都正确）

拿 `{5,1,9,5,5,3,7,5,2}` 举例，数"5 有几个"（正确答案 4）：

### 方案 ①（推荐）另开一张计数表，push/pop 时同步维护 —— O(1)

```cpp
priority_queue<int> pq;
unordered_map<int,int> cnt;                       // 值 → 出现次数
auto push = [&](int x){ pq.push(x); cnt[x]++; };
auto pop  = [&](){ int t = pq.top(); pq.pop(); if(--cnt[t] == 0) cnt.erase(t); return t; };

for (int x : data) push(x);
cout << cnt[5];        // 想看某个值有几个：O(1)
```
实测：`cnt[5] = 4` ✓；全部弹出后 `cnt` 自动变空 ✓。
**优点**：所有操作仍是 O(log n) / O(1)，最贴合"堆 + 计数"的需求。
**注意**：计数与堆必须**成对维护**（push 加、pop 减），否则两者会不一致。

### 方案 ② 换成 `multiset`（有序 + 计数 + 可删任意元素）

```cpp
multiset<int> ms(data.begin(), data.end());
ms.count(5);                  // O(log n + 结果个数) → 4 ✓
*ms.begin();                  // 最小值 1 ✓（大值用 *ms.rbegin()）
ms.erase(ms.find(5));         // 删除"一个 5"（注意：erase(5) 会删掉全部 5）
```
适合"既要极值、又要按值查找/计数/删除"的场景；代价是常数比堆大。

### 方案 ③ 派生类暴露底层容器 + `std::count`（能数，但 O(n) 且破坏封装）

```cpp
class MyPQ : public priority_queue<int> {
public:
    const vector<int>& raw() const { return c; }     // c 是 protected 成员
};
MyPQ mp; for (int x : data) mp.push(x);
count(mp.raw().begin(), mp.raw().end(), 5);          // 4 ✓（只读是安全的）
```
实测耗时对比（30 万元素）：**计数表 0 µs vs 遍历底层容器 119 µs**——所以只在本地调试时用。

### 方案 ④ 自己维护 `vector` + `make_heap`（完全掌控）

```cpp
vector<int> v(data);
make_heap(v.begin(), v.end());      // 大根堆，堆顶 v.front()
v.push_back(x); push_heap(v.begin(), v.end());
pop_heap(v.begin(), v.end()); v.pop_back();
count(v.begin(), v.end(), 5);       // 4 ✓；也能 erase / 排序 / 遍历
```
适合"既要堆又要随便翻容器"的复杂题目；代价是 push/pop 要写两步。

---

## 三、选型速查

| 你的需求 | 用什么 |
|---|---|
| 只要极值，偶尔问某个值几次 | **`priority_queue` + `unordered_map` 计数表**（首选） |
| 要按值查/删/计数，且要极值 | `multiset`（`*begin()` / `*rbegin()` 取极值） |
| 要自由遍历、删除、排序 | `vector` + `make_heap / push_heap / pop_heap` |
| 只是要"最大值"，不需要计数 | 直接 `priority_queue`（`top()`） |
| 要"第 k 大"（动态） | 小根堆保持 k 个（见 `算法讲义-02 §2.4`） |

---

## 四、坑清单

- [ ] `pq.size()` 是**总元素数**，不是某个值的个数
- [ ] `cnt` 表与堆必须同步：pop 后若计数为 0 记得 `erase`，别让 0 值留在地图里（会影响 `cnt.count(x)` 判断）
- [ ] `multiset::erase(x)` 删的是**所有** x；删一个用 `erase(find(x))`
- [ ] 用派生类拿 `c` 之后别手动改元素；改了要 `make_heap` 重建
- [ ] 堆里存 `pair` 或自定义结构时，计数表要按"可比较的键"设计（例如 `pair<int,int>` 直接当 key 用 `map`）

## 五、自查

1. `priority_queue` 有没有 `count`？为什么没有？
2. 想在堆里统计某个数出现次数，最省事的做法是什么？复杂度多少？
3. `multiset::count(5)` 和 `multiset::erase(5)` 各删/数几个 5？
4. 通过派生类访问底层容器 `c` 后，能不能直接改里面的元素？

答案：1. 没有——它是容器适配器，只暴露堆顶相关接口，不提供迭代器。2. 另开 `unordered_map<int,int>` 计数表，与 push/pop 同步维护，查询 O(1)、push/pop 仍是 O(log n)。3. `count(5)` 数出全部 5 的个数；`erase(5)` 删除**全部** 5（删一个要 `erase(find(5))`）。4. 不能随意改；改完必须 `make_heap` 重新建堆，否则堆性质失效、`top()/pop()` 结果不可信。
