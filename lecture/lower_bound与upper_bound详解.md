# lower_bound / upper_bound 详解：返回值、四种查找与边界

> 前置：`机试代码模板速查手册` §3.2（二分）、`迭代器prev详解`（`prev` / `next` / `advance`）、`CF1805C-最近邻二分与TLE排查`（"只查左右邻居"的实战用法）、`size_t详解`（下标类型）。
> 演示程序：`test_\32.cpp`（全部结论已用 g++ 16.2 / C++17 / -O2 实测，原始输出见文末附录）。

---

## 0. 一句话本质

两者返回的都是 **"x 在有序区间里的插入位置"**：

| 函数 | 语义 | 返回值的取值范围 |
|---|---|---|
| `lower_bound(first, last, x)` | **第一个 ≥ x** 的元素 | `[first, last]`（**右端闭**：可能是 `last`） |
| `upper_bound(first, last, x)` | **第一个 > x** 的元素 | `[first, last]`（同样可能是 `last`） |

**找不到时返回 `last`（即 `end()`）** —— 这是它们与 `find` 一致的地方，也是新手最容易忘记判的一段。

## 1. 返回值语义 + 基础实测表

`v = {1, 3, 3, 3, 7}`（`test_\32.cpp` 实测）：

| x | `lower_bound` | `upper_bound` | `upper − lower`(x 出现次数) |
|---|---|---|---|
| 0 | pos0 val1 | pos0 val1 | 0 |
| 1 | pos0 val1 | pos1 val3 | 1 |
| 3 | **pos1 val3**(第一个 3) | **pos4 val7**(所有 3 之后) | **3** ✅ |
| 5 | pos4 val7 | pos4 val7 | 0 |
| 7 | pos4 val7 | **end()** | 1 |
| 9 | **end()** | **end()** | 0 |

两条读表结论：

- **`x` 比所有元素都大**（x = 9）→ `lower_bound` 返回 `end()`；这是本题主题;
- **`x ≥ 最大元素`**（x = 7）→ `upper_bound` 也返回 `end()`（因为不存在"第一个 > 7"）；
- 出现次数 = `upper_bound(x) - lower_bound(x)`，**x 不存在时结果为 0**（下标 5−5 = 0，安全，不需要额外判 `end()`）。

## 2. 四种查找：前驱 / 后继全套

竞赛里除了"第一个 ≥ / 第一个 >"，还常要"**最后一个 <** x"和"**最后一个 ≤** x"，它们就是前者**往前一格**：

```cpp
auto lo = lower_bound(v.begin(), v.end(), x);   // 第一个 >= x
auto up = upper_bound(v.begin(), v.end(), x);   // 第一个 >  x
bool hasLt = (lo != v.begin());                 // 最后一个 <  x：prev(lo)，需判非 begin
bool hasLe = (up != v.begin());                 // 最后一个 <= x：prev(up)，需判非 begin
```

`test_\32.cpp` 实测（同一 `v = 1 3 3 3 7`）：

| x | 第一个 ≥ x | 第一个 > x | 最后一个 < x | 最后一个 ≤ x |
|---|---|---|---|---|
| 0 | pos0 val1 | pos0 val1 | **none** | **none** |
| 3 | pos1 val3 | pos4 val7 | pos0 val1 | **pos3 val3**(最后一个 3) |
| 5 | pos4 val7 | pos4 val7 | pos3 val3 | pos3 val3 |
| 9 | **end()** | **end()** | pos4 val7 | pos4 val7 |

> **要点**：`prev(lo)` / `prev(up)` 在 `lo == begin()`（没有任何元素 < x）时是非法的（`prev(begin())` 越界 UB），**必须先判**。这就是需要 `迭代器prev详解` 那份讲义的原因。

## 3. 三种边界情况（都是合法返回，不是错误）

`test_\32.cpp` 实测：

| 情形 | 结果 |
|---|---|
| **空容器** | `lower_bound(...) == end()`，且此时 `begin() == end()` 也成立（实测两个判断都为 1） |
| **x 比所有元素都小**（`{10,20,30}` 查 5） | `lower_bound` 与 `upper_bound` **都返回 `begin()`**（两种情况实测都为 1） |
| **x 比所有元素都大**（`{1,2,3}` 查 9） | `lower_bound` 与 `upper_bound` **都返回 `end()`**（实测都为 1） |

**返回 `end()` 本身完全合法**（它表示"该插在末尾"）——**只有解引用它才是 UB**。

## 4. 常见坑

| # | 坑 | 说明 / 正确写法 |
|---|---|---|
| ① | **解引用前不判 `end()`** | `auto it = lower_bound(...); *it` ✗ → `if (it != v.end() && *it == x)` ✅ |
| ② | **`pos = it - v.begin()` 拿去访问元素** | x 不存在时 `pos == n`（实测 x=9 → `pos = 5`，而 `v.size() == 5`）。`pos` 作为**插入位置**合法，`v[pos]` **越界** |
| ③ | **区间没排序** | 结果无保证（不报错，答案莫名错）。前提是"按**同一个比较器**划分好"，自定义 `cmp` 时两边必须一致 |
| ④ | **对 `set`/`map` 用全局 `lower_bound`** | 全局版是**线性**的 O(n)：实测 n=200000、q=2000 时 **3478.02 ms**，成员版 `s.lower_bound(x)` 只要 **0.85 ms**（约 **4000 倍**，见第 6 节） |
| ⑤ | **用 `lower_bound` 判存在却忘了比较值** | x=4 时 `lower_bound` 指向 val7（第一个 > 4），只看 `it != end()` 会误判"存在"。要么补 `*it == x`，要么直接 `binary_search`（返回 `bool`） |
| ⑥ | **拿 `upper_bound - begin()` 当"最后一个 ≤ x 的下标"** | 它其实是"第一个 > x 的下标"；**最后一个 ≤ x 是 `prev(upper_bound(...))`** |

## 5. 实战：最近邻候选的三种写法

场景（来自 `CF1805C`）：在有序数组 `k` 里找**离 `b` 最近**的元素，只需看 `b` 的左右邻居。

**写法 A：紧凑版**（利用"哨兵占位 + 过滤"）

```cpp
auto it = lower_bound(k.begin(), k.end(), b);                  // 第一个 >= b
for (auto cand : {it, it == k.begin() ? k.end() : prev(it)}) { // 右邻居 + 左邻居
    if (cand == k.end()) continue;                             // 挡掉哨兵与缺失的右邻居
    ... // 用 *cand 做判断
}
```

拆开看三部分：`{it, ...}` 是长度 2 的 `initializer_list`；三元表达式在"没有左邻居"时用 `k.end()` **占位**；`continue` 把占位（以及 `b` 比所有元素都大时的 `it == end()`）过滤掉。

**写法 B：下标版**（最易读，推荐日常使用）

```cpp
int p = int(lower_bound(k.begin(), k.end(), b) - k.begin());   // 第一个 >= b 的下标
for (int idx : {p - 1, p}) {
    if (idx < 0 || idx >= (int)k.size()) continue;             // 边界一目了然
    ...
}
```

**写法 C：两个 if**（最不容易绕晕）

```cpp
if (it != k.end())   { ... *it ... }        // 右邻居
if (it != k.begin()) { ... *prev(it) ... }  // 左邻居
```

实测候选（`k = -9 -4 -1 2 3 7 12 15 21 30`）：

| b | `pos = it - begin()` | 取到的候选 |
|---|---|---|
| 0 | 3 | `2` `-1` |
| −100 | 0（`it == begin()`） | `-9`、**[skipped]**（无左邻居） |
| 100 | 10（`it == end()`） | **[skipped]**、`30` |
| 15 | 7 | `15` `12`（b 正好等于某个 k） |
| −9 | 0（`it == begin()`） | `-9`、**[skipped]** |

**结论**：三种写法完全等价（`CF1805C` 那题已用 200 组带判题器对拍验证），差别只在可读性——**初学阶段建议写 B**。

## 6. `set` / `map` 必须用成员版（实测差 4000 倍）

`set` / `map` 是**平衡树**，没有随机访问迭代器，**全局 `lower_bound` 只能线性前进**（O(n)）；成员版走树结构（O(log n)）。

`test_\32.cpp` 实测（n = 200000，q = 2000 次查询，结果完全一致）：

| 写法 | 耗时 |
|---|---|
| `s.lower_bound(x)`（成员版，O(log n)） | **0.85 ms** ✅ |
| `lower_bound(s.begin(), s.end(), x)`（全局版，O(n)） | **3478.02 ms** ❌（约 4000 倍） |

> 顺带记进手册 §2.5 的那句提醒：**成员版不是"优化技巧"，而是正确用法**。

## 7. 习题

**E1** `v = {2, 4, 4, 9}`，写出 `lower_bound(v, 4)` 与 `upper_bound(v, 4)` 的值和下标。

**E2** 判断对错：
```
A. lower_bound 找不到时返回 end()
B. lower_bound 的返回值一定可以解引用
C. x 比所有元素都大时，upper_bound 也返回 end()
D. pos = lower_bound(...) - v.begin() 一定小于 v.size()
```

**E3** 写一行代码：求有序 `v` 中**最后一个 ≤ x** 的元素（不存在时说明判据）。

**E4** 要回答"`v` 中是否存在 x"，用哪个函数最省事？要同时拿到位置呢？

**E5** `set<int> s`，下面哪种写法在 q = 1e5 次查询时能过？为什么？
```cpp
A. lower_bound(s.begin(), s.end(), x);
B. s.lower_bound(x);
```

---

## 答案与解析

**E1** → 排序后 `2 4 4 9`：`lower_bound(4)` = **pos1 val4**（第一个 4）；`upper_bound(4)` = **pos3 val9**（所有 4 之后）；出现次数 = 3−1 = 2。

**E2** → **A、C 对；B、D 错**。B：`end()` 不可解引用（UB）；D：x 比所有元素都大时 `pos == v.size()`（实测 `v = 1 3 3 3 7` 查 9 → `pos = 5 = n`）。

**E3** →
```cpp
auto up = upper_bound(v.begin(), v.end(), x);
if (up != v.begin()) { auto it = prev(up); /* *it 就是最后一个 <= x */ }
```

**E4** → 只判存在用 `binary_search(v.begin(), v.end(), x)`（返回 `bool`，不需要判 `end()`）；要位置则 `auto it = lower_bound(...); bool ok = (it != v.end() && *it == x);`（实测 x=3 → 两者都为 1；x=4 → 都为 0）。

**E5** → **B**。A 是全局版 O(n)，q = 1e5 次 → O(nq)；B 是成员版 O(log n)。实测 q=2000 时 A 已要 3.5 秒，B 只要 0.85 毫秒。

---

**一句话总结**：`lower_bound` / `upper_bound` 返回的是**插入位置**：前者"第一个 ≥ x"、后者"第一个 > x"，**没有这样的元素就返回 `end()`**——所以"解引用前先判 `end()`"是铁律；`it - begin()` 在找不到时等于 `n`（可作插入位置、不能访问元素）；要前驱（最后一个 < / ≤）就用 `prev` 并判 `begin()`；**`set`/`map` 一律用成员版**（实测差 4000 倍）。

---

## 附录：`test_\32.cpp` 原始输出（g++ 16.2 / C++17 / -O2）

```
=== (1) basic table, v = 1 3 3 3 7 ===
x    lower_bound(>=x)     upper_bound(>x)      count = ub-lb
0    pos0 val1            pos0 val1            0
1    pos0 val1            pos1 val3            1
3    pos1 val3            pos4 val7            3
5    pos4 val7            pos4 val7            0
7    pos4 val7            end()                1
9    end()                end()                0

=== (2) four queries ===
x=0   first>=: pos0 val1  first>: pos0 val1  last<: none       last<=: none
x=3   first>=: pos1 val3  first>: pos4 val7  last<: pos0 val1  last<=: pos3 val3
x=5   first>=: pos4 val7  first>: pos4 val7  last<: pos3 val3  last<=: pos3 val3
x=9   first>=: end()      first>: end()      last<: pos4 val7  last<=: pos4 val7

=== (3) edge cases (all return end() / begin()) ===
empty:      lower_bound(1)==end? 1  (此时 begin()==end() 也成立: 1)
all > x:    lower_bound(5)==begin? 1   upper_bound(5)==begin? 1
all < x:    lower_bound(9)==end?   1   upper_bound(9)==end?   1

=== (4) pos = it - begin() can be n ===
x=9 -> pos = 5 (v.size() = 5)  => pos is a valid INSERT position but v[pos] is out of bounds

=== (5) existence: binary_search vs lower_bound ===
x=3: binary_search=1   lower_bound+check=1
x=4: binary_search=0   lower_bound+check=0

=== (6) neighbor candidates: {it, it==begin()? end() : prev(it)} ===
b=    0  pos= 3  cands: 2 -1
b= -100  pos= 0  cands: -9 [skipped]
b=  100  pos=10  cands: [skipped] 30
b=   15  pos= 7  cands: 15 12
b=   -9  pos= 0  cands: -9 [skipped]

=== (7) set/map member version vs global (n=200000, q=2000) ===
member  s.lower_bound(x)      :     0.85 ms  (sum=404520784)
global  lower_bound(s.begin..) :  3478.02 ms  (sum=404520784)  <- O(n) per query!
same results? yes
```

> 复现：`g++ -std=c++17 -O2 -o test_\32.exe test_\32.cpp && test_\32.exe`（耗时随机器浮动，量级与结论不变）。
