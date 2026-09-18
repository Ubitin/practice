# pair 详解：打包两个值

> 前置：STL 讲义 §5（pair 基础）、`洛谷P1553` 思路（pair 排序是"双关键字"的现成工具）。
> 演示程序：`test_\11.cpp`（全部结论已用 g++ C++17 实测）。

---

## 0. 一句话本质

**`pair<T1, T2>` 把两个值打包成一个"整体"**，自带比较规则、能进容器、能当函数返回值——是"双关键字"问题的天然载体。

## 1. 声明与初始化（4 种方式）

```cpp
pair<int, int> p1 = {1, 2};             // ① C++11 花括号（最常用，推荐）
pair<int, int> p2 = make_pair(3, 4);    // ② 旧写法（C++11 之前只能用这个）
pair<int, int> p3(5, 6);                // ③ 构造函数
auto p4 = make_pair(7, 8);              // ④ auto + make_pair，类型自动推
```

## 2. 访问与修改：first / second

```cpp
p1.first;     // 第一个值
p1.second;    // 第二个值
p1.first = 100;   // 可以改（pair 不是只读的）
```

## 3. 比较规则（本讲义最重要的考点）

**先比 `first`，first 相同再比 `second`**（字典序），实测：

```cpp
(1,2) < (1,3)   // true  —— first 相同(1==1)，比 second(2<3)
(1,2) < (2,1)   // true  —— first 直接决定(1<2)，不看 second 了
(1,2) == (1,2)  // true  —— 两个都相等才相等
```

> 记忆钩子：**pair 比较 = 查字典**——先看第一个字，相同再看第二个字。`<` `>` `<=` `>=` `==` 全都按这个规则工作。

## 4. 排序：sort 自动双关键字（最常用）

```cpp
vector<pair<int, int>> v = {{3,9}, {1,5}, {2,8}, {1,2}};
sort(v.begin(), v.end());          // 不需要 cmp！
// 结果: (1,2) (1,5) (2,8) (3,9)   —— first 升序，first 同的按 second
```

**这就是"按分数排、同分按学号排"的一行解**——你之前要写 cmp 结构体，用 pair 直接免了。

## 5. 四个典型应用（都实测过）

**① 优先队列：按 first 取最大**
```cpp
priority_queue<pair<int,int>> pq;   // 大根堆：first 大的优先，first 同比 second
pq.push({3,9}); pq.push({1,5}); pq.push({3,2});
pq.top();   // (3,9)
```

**② map 的二维键**
```cpp
map<pair<int,int>, string> m;
m[{2,3}] = "点(2,3)";        // 两个维度当键（如坐标 → 名字）
m[{1,1}] = "原点附近";
// 遍历：kv.first 是 pair（.first/.second），kv.second 是值
```

**③ 函数返回两个值 + 结构化绑定（C++17）**
```cpp
pair<int,int> makePoint(int x, int y) { return {x, y}; }
auto p = makePoint(7, 8);            // (7,8)
auto [x, y] = makePoint(10, 20);     // 结构化绑定：x=10, y=20，直接拆开
```

**④ 图论存边 / 点集**
```cpp
vector<pair<int,int>> edges;
edges.push_back({1, 2});       // 存一条边 1-2
edges.emplace_back(3, 4);      // emplace_back：原地构造，比 push_back 略快
```

## 6. 常见坑

1. **比较规则记反**：是"first 优先"，不是"second 优先"。想按 second 排得自定义 cmp 或用 `make_pair(x.second, x.first)` 换位。
2. **旧编译器初始化**：`{1,2}` 需要 C++11；老环境（C++98）只能用 `make_pair`。评测机一般是 C++17，放心用花括号。
3. **priority_queue 默认大根堆**：想按 first 取最小，要 `priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>>`。
4. **结构化绑定需 C++17**：`auto [x,y] = p;` 在 C++14 会编译失败；不确定环境就老实写 `p.first/p.second`。

## 7. 习题（答案已实测）

**E1** 写出输出：
```cpp
sort 后: vector<pair<int,int>> v = {{1,9},{1,2},{2,1},{0,5}};
for (auto& p : v) cout << "(" << p.first << "," << p.second << ") ";
```

**E2** 判断：`(2,1) < (1,9)` 结果是 true 还是 false？为什么？

**E3** 补全：把 `vector<pair<int,int>> pts` 按 **second 升序**（second 相同按 first）排序，怎么写？

**E4** 选择：`priority_queue<pair<int,int>>` 依次 push `(1,5) (2,3) (2,7)`，top() 是？
A. (1,5)  B. (2,3)  C. (2,7)  D. 不确定

**E5** 读程序写输出：
```cpp
map<pair<int,int>, string> m;
m[{1,2}] = "a";  m[{1,1}] = "b";
for (auto& kv : m) cout << kv.first.first << kv.first.second << kv.second << " ";
```

---

## 答案与解析

**E1** → `(0,5) (1,2) (1,9) (2,1)`
先按 first：0 < 1 < 2；first=1 的两个按 second：2 < 9。

**E2** → **false**。先比 first：2 < 1 不成立，直接 false——second 根本没机会比较。

**E3** → 三种写法：
```cpp
// ① 换位法（最简单，排序完记得换回来）
for (auto& p : pts) swap(p.first, p.second);
sort(pts.begin(), pts.end());
for (auto& p : pts) swap(p.first, p.second);
// ② 自定义 cmp
sort(pts.begin(), pts.end(), [](const auto& a, const auto& b){
    if (a.second != b.second) return a.second < b.second;
    return a.first < b.first;
});
// ③ 存进 map<int,int>（second 作键）——不通用，略
```

**E4** → **C (2,7)**。大根堆先比 first：2 最大（两个 2 之一）；再比 second：7 > 3，所以 (2,7)。

**E5** → `11b12a`
map 按键升序：`(1,1)` < `(1,2)`（first 相同比 second），所以先输出 (1,1) 的 "b"，再 (1,2) 的 "a"。

---

**一句话总结**：pair = **"双值打包 + 字典序比较"**——`{first, second}` 初始化、`sort` 免 cmp 双关键字排序、`priority_queue` 按 first 优先、`map<pair,int>` 做二维键，是竞赛"点对/边/双关键字"题型的万能零件。

---

## 附：习题批改记录（2026-08-16 · 首刷 2/5）

> 作答文件：`test_\answer.txt`。三道错题暴露两个概念断层，见下方"错题归因"。

| 题 | 你的答案 | 正确答案 | 判定 |
|---|---|---|---|
| E1 排序输出 | (0,5)(1,2)(1,9)(2,1) | 同左 | ✅ |
| E2 比较规则 | false，先比 first | false（first 直接决定） | ✅ |
| E3 按 second 排序 | 方向反 + `firsr` 拼写错 + 缺兜底 | second/first 均升序 | ❌ |
| E4 priority_queue top | A (1,5) | **C (2,7)** | ❌ |
| E5 map 遍历 | 12a 11b（插入序） | **11b 12a**（按键序） | ❌ |

### 错题解析

**E3** 三个问题：① `cmp` 返回 true = "p1 排前面"，要升序应写 `<`（你写成 `>` 降序）；② `firsr` 拼写错误直接编译失败；③ 两元素相等时无兜底 `return false`（未定义行为）。正确写法：
```cpp
bool cmp(const pair<int,int>& a, const pair<int,int>& b) {
    if (a.second != b.second) return a.second < b.second;   // second 升序
    return a.first < b.first;                               // 同 second 按 first 升序
}
```

**E4** 把 priority_queue 当成了 queue：`push (1,5)(2,3)(2,7)` 后大根堆先比 first（2 最大），同 first 再比 second（7>3），top() = **(2,7)**。queue 按插入序、priority_queue 按最值——概念区分。

**E5** map 遍历严格按 key 升序（红黑树）：键 (1,1) < (1,2)，先输出 (1,1) 的 "b" → `11b 12a`。想按插入序遍历要用 vector。

### 错题归因

1. **比较器方向未内化**："return true = 排前面"、升序用 `<`——E3 的根因。
2. **三类容器的"顺序"混淆**：queue 按插入、priority_queue 按大小、map 按键大小——E4/E5 的根因。

### 复习动作

1. 默写 E3 的升序 cmp（含兜底 return false）；
2. 手推一遍 priority_queue 堆顶选择过程（first 最大 → second 最大）；
3. 把 map 遍历顺序想成"查字典"，不是"排队"。
