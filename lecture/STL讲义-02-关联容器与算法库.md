# STL 讲义 · 第二讲：set / map、无序容器与算法库

> 依据 OI Wiki《C++ 标准库》页（https://oi-wiki.org/lang/csl/）的关联式容器、无序关联式容器、迭代器、算法等章节整理并筛选。
> 上一讲：`STL讲义-01-基础容器.md`。下一讲：`STL讲义-03-两周冲刺计划.md`。

---

## 1. set —— 自动排序 + 自动去重的集合

想象一个盒子，数字放进去后**自动从小到大排好、且不会有重复**。这就是 set。

```cpp
#include <bits/stdc++.h>
using namespace std;
int main() {
    set<int> s;
    s.insert(3); s.insert(1); s.insert(3); s.insert(2);
    for (int x : s) cout << x << " ";   // 输出 1 2 3 —— 有序且 3 只出现一次
    cout << s.size() << "\n";           // 3

    // 判断有没有：count(x) 返回 0 或 1（set 里元素唯一）
    if (s.count(2)) cout << "有 2\n";

    // find(x)：返回指向 x 的迭代器；找不到返回 end()
    if (s.find(5) == s.end()) cout << "没有 5\n";

    s.erase(1);                         // 删除元素 1
    return 0;
}
```

### 要点

- 底层是红黑树，插入 / 删除 / 查找都是 **O(log n)**。
- **遍历出来天然有序**——"去重 + 排序"一步到位，这是它和 vector + sort 相比最方便的地方。
- 找"第一个 ≥ x 的元素"：`auto it = s.lower_bound(x);`（set 自带 lower_bound，比全局的更快）。

**例题**：洛谷 **P1059 明明的随机数**（输入 → 丢进 set → 输出，一行循环搞定）；进阶 **P5250【深基17.例5】木材仓库**。

## 2. multiset —— 允许重复的 set

和 set 操作几乎一样，但**允许重复元素**，`count(x)` 返回实际个数。用途：需要"动态维护有序、可重复"的数据。
**例题**：**P1090 合并果子** 用 multiset 也可以做（和 priority_queue 对比着写一遍，加深理解）。

> 很少用到 multimap（允许重复 key 的 map），OI Wiki 也只是一带而过。需要"一个 key 对应多个值"时，用 `map<int, vector<int>>` 更顺手，先跳过。

## 3. map —— 键值对（字典）

可以理解成"**下标可以不是数字的数组**"：把任意类型的 key（常见 int / string）映射到一个 value。

```cpp
map<string, int> m;      // 名字 → 分数
m["Alice"] = 90;
m["Bob"] = 85;
cout << m["Alice"];      // 90
m["Alice"]++;            // 也可以直接运算

// 判断某个 key 是否存在
if (m.count("Cindy")) ...
if (m.find("Cindy") != m.end()) ...

// 遍历：按 key 从小到大自动排好
for (auto& p : m)
    cout << p.first << " " << p.second << "\n";   // Alice 90 / Bob 85
```

### 头号大坑（OI Wiki 反复强调）

**`m[key]` 在 key 不存在时会"自动创建"一个默认值（int 是 0）并插入！** 所以：

- 只是想"查有没有"：用 `count` 或 `find`，别用 `m[x]`；
- 直接赋值 / 累加：用 `m[x]` 反而方便。

复杂度同样是 O(log n)。**例题**：洛谷 **P1918 保龄球**、**P5266【深基17.例6】学籍管理**。

## 4. unordered_map / unordered_set —— 哈希表（不排序，但更快）

和 map / set 功能对应，差别只有两条：

1. **无序**：不能按 key 从小到大遍历；
2. **平均 O(1)，最坏 O(n)**：底层是哈希表，绝大多数时候比 map 快得多。

```cpp
unordered_map<string, int> um;
um["Alice"] = 90;
if (um.count("Bob")) ...
```

### 选型口诀

- 只需要"查有没有 / 查值"，**不在乎顺序** → 用 `unordered_map` / `unordered_set`（快）；
- 需要按 key 有序遍历、或求 lower_bound → 用 `map` / `set`；
- 个别毒瘤题目会构造数据故意让哈希表退化成 O(n)（被"卡哈希"），此时换回 map 保底。入门阶段不用纠结，先用 unordered_map。

> 过滤掉的内容：自定义哈希函数、rehash、负载因子（OI Wiki 无序容器页后面这些内容）——属于调优级别，进队后再看。

## 5. 迭代器要点补完（OI Wiki 迭代器章）

- **set / map 的迭代器只能 `++` / `--`，不能 `it + 3`**（底层是树，没法跳）；vector 的迭代器可以 `it + k`（底层是连续内存）。
- 反向遍历：`rbegin()` 指向最后一个元素，`++` 往反方向走；`rend()` 是"第一个元素之前"。
- **迭代器失效**：vector 在 push_back 时若发生扩容，元素会整体搬家，之前保存的迭代器就作废了。新手守则：**迭代器随用随取（每次现写 `v.begin()`），不要存下来长期持有。** set 中 `erase(it)` 后，被删元素对应的迭代器也作废（C++11 起 erase 返回下一个有效迭代器）。
- 结论：会用范围 for + 现取 begin/end，就避开了 90% 的坑。

## 6. algorithm —— 算法库（本讲核心，机试得分主力）

OI Wiki 算法页介绍：STL 提供了约 100 个算法函数，绝大多数在 `<algorithm>` 里（万能头已包含）。下面按"考试价值"从高到低讲。

### 6.1 sort —— 排序（必考）

```cpp
vector<int> v = {3, 1, 4, 1, 5};
sort(v.begin(), v.end());                     // 升序，O(n log n)
sort(v.begin(), v.end(), greater<int>());     // 降序

int a[100005];                                // 普通数组一样能用
sort(a + 1, a + n + 1);                       // 排 a[1] ~ a[n]（注意左闭右开！）
```

**自定义比较函数（结构体排序，必学）**：

```cpp
struct Stu { int id, score; };
bool cmp(const Stu& a, const Stu& b) {
    if (a.score != b.score) return a.score > b.score;  // 分数高的在前
    return a.id < b.id;                                // 同分，学号小的在前
}
vector<Stu> v;
sort(v.begin(), v.end(), cmp);
```

口诀：**cmp(a, b) 返回 true，表示"a 应该排在 b 前面"。**
变体：结构体里写 `operator<`（OI Wiki 有示例），作用相同，先学 cmp 一种即可。

`stable_sort`：稳定排序（相同元素保持原相对顺序），OI 里用得少，知道名字就行。
**例题**：**P1177【模板】排序**、**P1068 分数线划定**（普及组真题，结构体排序）、**P1781 宇宙总统**（string 比较 + 排序，好题）。

### 6.2 unique + erase —— 去重三连（背下来）

`unique` **只去掉相邻的重复**，所以必须先 sort。它返回去重后区间的"新末尾"迭代器，配合 `erase` 把多余部分删掉：

```cpp
sort(v.begin(), v.end());
v.erase(unique(v.begin(), v.end()), v.end());   // 去重三连，直接背
```

**例题**：**P1059 明明的随机数**——用这个方法再写一遍，和 set 的写法对比。

### 6.3 lower_bound / upper_bound —— 二分查找（必考）

前提：数组**已经有序**。复杂度 O(log n)。

- `lower_bound(first, last, x)`：返回**第一个 ≥ x** 的元素的位置；
- `upper_bound(first, last, x)`：返回**第一个 > x** 的元素的位置；
- 都没找到时返回 `last`（对 vector 就是 `end()`）。

```cpp
auto it = lower_bound(v.begin(), v.end(), x);
if (it != v.end() && *it == x) {           // 注意判断：lower_bound 找到的是 ≥ x
    int pos = it - v.begin();              // 迭代器相减 → 下标
    cout << pos;                           // 0 开始的下标
}
int cnt = upper_bound(v.begin(), v.end(), x) - lower_bound(v.begin(), v.end(), x);
// cnt = x 的出现次数
```

`binary_search(first, last, x)`：只判断"存不存在"，返回 bool。
**例题**：**P2249【深基13.例1】查找**（输出第一次出现的位置 = lower_bound + 相等判断）；进阶 **P1678 烦恼的高考志愿**。

### 6.4 next_permutation —— 全排列（模板背下来）

```cpp
int a[] = {1, 2, 3};
sort(a, a + 3);                      // 一定先从最小排列开始
do {
    // 处理当前排列 a
    cout << a[0] << a[1] << a[2] << "\n";
} while (next_permutation(a, a + 3));
// 输出 123 132 213 231 312 321
```

注意：它按**字典序**依次生成下一个排列；若已经到最大排列则返回 false 并结束。`prev_permutation` 反向，几乎用不到。
**例题**：**P1706 全排列问题**、**P1088 火星人**（经典应用）。

### 6.5 其他高频小工具

```cpp
reverse(v.begin(), v.end());            // 翻转
auto it = max_element(v.begin(), v.end());   // 最大元素的迭代器
cout << *it;                            // 取它的值
*max_element(v.begin(), v.end());       // 最小值用 min_element
swap(a, b);                             // 交换
fill(v.begin(), v.end(), 0);            // 区间批量赋值
max(a, b); min(a, b);                   // 两数最值
std::gcd(a, b);                         // C++17 最大公约数（万能头包含 <numeric>）
```

> 选学：`nth_element` 找第 k 小（**P1923 模板**）；`merge` 合并两个有序区间。时间紧可跳过。

## 7. 容器选型速查表（考前再看一眼）

| 你的需求 | 选它 |
|---|---|
| 存一堆数，末尾加删，按下标访问 | `vector` |
| 处理文本 | `string` |
| 后进先出 | `stack` |
| 先进先出 | `queue` |
| 反复取最大 / 最小 | `priority_queue` |
| 去重 + 排序 | `set`（或 sort + unique） |
| 动态插入删除，还要保持有序 | `set` / `multiset` |
| 编号 / 名字 → 值，需要有序 | `map` |
| 编号 / 名字 → 值，只要快 | `unordered_map` |
| 只查"存不存在"，要快 | `unordered_set` |
| 两端都要快速加删（滑动窗口等） | `deque`（第三讲进阶） |
| 两个值打包 / 双关键字排序 | `pair` |

## 8. 新手最容易踩的 12 个坑（浓缩版）

1. **`v[i]` 越界不报错**：每次下标访问前确认 `i < v.size()`。
2. **`end()` 不能解引用**：它是"最后一个元素的后面"，`*v.end()` 是灾难。
3. **pop 不返回值**：`stack::pop`、`queue::pop` 都只是删除；先 `top()/front()` 取到值再 pop。
4. **取元素前判空**：栈空还 `top()`、队空还 `front()` = 运行时错误。
5. **priority_queue 默认大根堆**：要最小值记得写 `priority_queue<int, vector<int>, greater<int>>`。
6. **`m[key]` 会偷偷插入**：判断存在性用 `count` / `find`。
7. **sort 的区间左闭右开**：数组是 `sort(a + 1, a + n + 1)`，别漏 +1。
8. **lower_bound 返回迭代器**：要下标就 `it - v.begin()`；"找到"要再判断 `*it == x`。
9. **unique 只去相邻重复**：必须先 sort。
10. **cin 与 getline 混用**：中间加 `cin.ignore()`（或 getchar）吃残留换行。
11. **迭代器失效**：vector 扩容后旧迭代器作废——迭代器随用随取。
12. **多组输入**：机试题常有 `while (cin >> n)`（读不到就结束）；输入完记得处理完这组就输出，别攒到最后。

## 9. 复杂度直觉（OI Wiki 复杂度章的精华）

机试经验法则：**1 秒约能做 1e8 次基本操作**。

| 数据规模 n | 你需要的最坏复杂度 |
|---|---|
| n ≤ 30 | 随便写，甚至可以暴力枚举 |
| n ≤ 1000 | O(n²) 没问题（冒泡也行） |
| n ≤ 1e5 | 至少 O(n log n)：sort / set / map / 二分 |
| n ≤ 1e6 | 基本要 O(n)，且常数要小 |

对应到 STL：`sort`、`set/map`、`priority_queue`、`lower_bound` 都是 O(log n) 级别的单次操作；vector 末尾操作、unordered_map 平均 O(1)。**见到 n ≥ 1e5 还敢写两层循环，就是等着超时。**

## 10. 被我过滤掉的内容（现在不学，理由如下）

OI Wiki CSL 页面里以下内容对"9 月 26 日机试"性价比太低，已删去：

- **list / forward_list（链表）**：OI 中几乎不用，常数大、代码长，需要链表时常用"数组模拟"或 vector 替代思路。
- **array**：定长数组的包装，用 C 数组即可，多一个概念没收益。
- **multimap / unordered_multiset / unordered_multimap**：出现率极低，用 map + vector 组合更顺手。
- **自定义哈希、rehash、负载因子**：调优级别。
- **元函数 metafunction（type_traits）**：模板元编程，和机试无关。
- **内存分配器 allocator**：库实现细节，知道"存在"即可。
- **bitset**：位集容器，省选级别位运算优化的利器——**进队后值得学**，现在先不碰。
- **random_shuffle**：C++17 已移除，别用（要乱序用 `shuffle`，现在也用不上）。
- **C++20 的 span 等新容器**：评测机环境未必支持，别冒险。

---

## 本讲小结

第二讲学完，你已经覆盖了机试 STL 的 95% 需求：

> `vector / string / pair` + `stack / queue / priority_queue` + `set / map / unordered_map / unordered_set` + `sort / unique / lower_bound / upper_bound / next_permutation` + 迭代器基础。

**今天的练习**：P1059（两种做法）、P1918、P2249、P1706、P1068。
剩下的事只有一件：**刷题把它变成肌肉记忆**——看 `STL讲义-03-两周冲刺计划.md`。
