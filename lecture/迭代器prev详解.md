# 迭代器 prev 详解 —— 往回走一步（或 n 步）

> 一句话：`prev(it, n = 1)` 返回"把 `it` **向前（朝开头方向）移动 n 步**"后的**迭代器副本**，
> **不改变 `it` 本身**。C++11 起可用，头文件 `<iterator>`（`bits/stdc++.h` 已含）。
> 与它成对的还有 `next`（朝后走）和 `advance`（就地移动，无返回值）。

---

## 一、原型与语义

```cpp
template <class BidirIt>
BidirIt prev(BidirIt it, typename iterator_traits<BidirIt>::difference_type n = 1);
```

- 返回值：移动后的**新迭代器**（副本）；
- `n` 默认 1；**`n` 可以是负数**——`prev(it, -2)` 等价于 `next(it, 2)`（朝后走 2 步）；
- **不修改传入的 `it`**（这点和 `advance` 相反）。

---

## 二、三兄弟对照（最容易混）

| 函数 | 方向 | 是否修改传入的迭代器 | 返回值 | 对迭代器类别的要求 |
|---|---|---|---|---|
| `advance(it, n)` | 正 n 向后、负 n 向前 | ✅ **就地修改** `it` | `void` | 输入迭代器即可（但负 n 需双向） |
| `next(it, n = 1)` | 向后（朝 `end()`） | ❌ 不改 | 新迭代器 | 前向迭代器（负 n 需双向/随机访问） |
| `prev(it, n = 1)` | **向前（朝 `begin()`）** | ❌ 不改 | 新迭代器 | **必须是双向迭代器**（因为要 `--`） |

**记忆**：`prev` / `next` 都是"只看不改"，`advance` 是"就地改"。

---

## 三、实测（g++ 16.2）

```cpp
vector<int> v{10,20,30,40,50};
```

| 表达式 | 结果 | 说明 |
|---|---|---|
| `*prev(v.end())` | `50` | 取最后一个元素（`v.end()` 本身没变 ✓） |
| `*prev(v.end(), 2)` | `40` | 往前 2 步 |
| `*prev(v.begin()+3, 3)` | `10` | 等价于 `v[0]` |
| `*prev(v.begin()+1, -2)` | `40` | **负 n = 朝后走** → 指向 `v[3]` |
| `prev(a)`（`a = v.begin()+2`） | `a` 仍指向 `30` | **不修改原迭代器** ✓ |
| `advance(c,-1)`（`c` 同位置） | `c` 指向 `20` | `advance` **改了自己** |
| `*prev(L.end())`（`list`） | `5` | 双向迭代器可用 ✓ |
| `*prev(S.lower_bound(4))`（`set`） | `3` | 取"小于 4 的最大元素"（前驱）✓ |

**复杂度实测**（移动 400000 步）：

```
vector: 0 µs        ← 随机访问迭代器：prev 直接 it - n，O(1)
list  : 1531 µs     ← 双向迭代器：只能一次次 --，O(n)
```

> 所以：`vector/string/deque/array` 上 `prev(it, n)` 是 **O(1)**；
> `list/set/map` 上是 **O(n)**（`prev(end())` 这种 n=1 的情况在红黑树/链表上仍是 O(1)~O(常数) 摊销）。

---

## 四、典型用法

```cpp
// ① 取末尾元素（等价 v.back()，但泛型代码里更通用）
auto last = prev(v.end());

// ② 取"上一步"的迭代器（反向遍历、相邻比较）
for (auto it = v.end(); it != v.begin(); ) {
    --it;                       // 或用 it = prev(it);
    cout << *it << " ";
}

// ③ 与 lower_bound 配合，取"第一个 ≥ x"的前驱（即 < x 的最后一个）
auto it = lower_bound(v.begin(), v.end(), x);
if (it != v.begin()) { auto pre = prev(it); cout << *pre; }   // ⚠️ 必须先判 != begin()

// ④ 删除时定位前一个元素
auto it = find(v.begin(), v.end(), 7);
if (it != v.end() && it != v.begin()) v.erase(prev(it));

// ⑤ 泛型函数里不依赖随机访问（对 list/vector 都成立）
template <class It> auto lastOf(It b, It e) { return prev(e); }
```

---

## 五、坑（都实测或编译验证过）

### ① 绝不能越过 `begin()`

`prev(v.begin())` 是**未定义行为**（哪怕你不解引用它也一样）。
正确写法永远先判：

```cpp
if (it != v.begin()) { auto pre = prev(it); ... }      // ✅
```

### ② 空容器陷阱

空容器里 `begin() == end()`，所以 `prev(v.end())` 同样越界：

```cpp
if (!v.empty()) { auto last = prev(v.end()); ... }     // ✅
```

### ③ 前向迭代器上不能用 —— 而且**编译器不会拦你**（实测）

`forward_list`、`unordered_map/set` 的迭代器是**前向**的，不满足双向要求，用 `prev` 是 UB。实测：

```
g++ -std=c++17  编译：通过（exit 0）——模板只看用到的操作，不检查迭代器类别！
运行：挂死（3 秒未结束被强制杀掉）
加 -D_GLIBCXX_DEBUG 编译：进程会提前结束（调试迭代器的检查起作用）
```

**教训：编译通过 ≠ 合法**。这类"迭代器类别不匹配"的错，靠编译器报错是发现不了的，要看标准要求。

### ④ `prev` 返回副本，别对它赋值

```cpp
prev(v.end()) = something;   // ❌ 给临时迭代器赋值，毫无意义
*prev(v.end()) = 99;         // ✅ 解引用后改元素没问题
```

### ⑤ C++11 起才有

C++98/03 里没有 `prev`/`next`（只有 `advance` 和 `--`）。老环境要写 `auto p = it; --p;`

### ⑥ `end()` 可以 `prev`，`begin()` 不可以

`end()` 是"最后一个元素的下一个位置"，允许递减；`begin()` 之前没有任何合法位置。

---

## 六、自查

1. `prev` 和 `advance` 最本质的区别是什么？
2. `*prev(v.end())` 与 `v.back()` 有何不同？（提示：空容器）
3. 为什么 `set` 上 `prev(it, 1000)` 比 `vector` 上慢？
4. `prev(forward_list.end())` 会怎样？编译器会报错吗？
5. `prev(it, -3)` 等价于什么？

答案：1. `prev` 返回移动后的副本、不修改原迭代器；`advance` 就地修改且无返回值。2. 结果相同（都是最后一个元素），但空容器时 `prev(v.end())` 是 UB，而 `v.back()` 在空容器上也是 UB——都要先 `!empty()`；泛型代码里 `prev(e)` 更通用。3. `set` 的迭代器是双向的，`prev` 只能靠反复 `--`，复杂度 O(n)；`vector` 是随机访问，直接 `it-n`，O(1)。4. UB；普通模式下**编译通过**但运行会挂死（实测），加 `-D_GLIBCXX_DEBUG` 才会被调试检查拦下。5. 等于 `next(it, 3)`（朝后走 3 步）。
