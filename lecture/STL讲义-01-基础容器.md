# STL 讲义 · 第一讲：基础容器（vector / string / pair / stack / queue / priority_queue）

> 依据 OI Wiki《C++ 标准库》页（https://oi-wiki.org/lang/csl/）整理，并已按你的情况（C++ 入门基础 + 9 月 26 日机试）做了筛选：**只讲机试会用到的，暂时用不到的已删去**。
> 配套文件：`STL讲义-02-关联容器与算法库.md`、`STL讲义-03-两周冲刺计划.md`。

---

## 0. 为什么要学 STL？—— 三个直白的例子

你已经有数组、结构体、函数这些工具了。但机试是有时间限制的，有些"基础操作"自己手写又慢又容易错：

| 你想做的事 | 手写 | 用 STL |
|---|---|---|
| 给 10 万个数字排序 | 手写排序（冒泡太慢，快排容易写错） | `sort` 一行，O(n log n) |
| 存"不知道有多少个"的数字 | 数组开固定大小，开小越界、开大浪费 | `vector` 自动变长 |
| 括号匹配、模拟排队 | 自己用结构体写栈/队列，指针细节易错 | `stack` / `queue` 现成、不会错 |

一句话：**STL 就是别人写好的、随编译器附赠的数据结构和算法工具箱。** 程序 = 数据结构 + 算法，数据结构那半，STL 帮你搞定了 80%。

## 1. STL 是什么（OI Wiki 页面怎么说）

STL = Standard Template Library（标准模板库）。OI Wiki 的 CSL 页面把标准库按功能分成：

- **容器（containers）**：装数据的东西 —— vector、set、map、stack…… ← 本讲和第二讲的主角
- **算法（algorithms）**：对数据做操作 —— sort、二分、去重…… ← 第二讲
- **迭代器（iterators）**：指向容器中元素的"位置标记" ← 本讲第 2 节
- 函数对象、适配器、内存分配器：**你现在不用管**，知道有这回事就行

### 头文件与命名空间

机试（GCC 环境）里最省事的写法，所有代码统一这样开头：

```cpp
#include <bits/stdc++.h>   // 万能头：一次包含几乎所有标准库，机试专用
using namespace std;       // 让 std::vector 可以简写成 vector
```

> 注：OI Wiki 按容器分别写 `<vector>` `<string>` `<stack>` `<queue>` `<algorithm>` `<set>` `<map>` 等头文件。写万能头完全没问题，几乎所有在线评测（含洛谷）都支持。

## 2. 迭代器：STL 的"位置标记"（你不需要懂指针）

你没学过指针没关系，这样理解就够了：

- 每个容器都有一串元素。**迭代器就是指向其中某个元素（或"末尾之后"）的一个标记**。
- `begin()` → 指向第一个元素的位置。
- `end()` → 指向**最后一个元素的后面**的位置（注意：不是最后一个元素本身！）。
- 区间的写法一律是**左闭右开 `[begin, end)`**：包含 begin，不包含 end。
- 对迭代器用 `++` 往后移一格；用 `*` 取出它指向的元素。

```cpp
vector<int> v = {10, 20, 30};

// 遍历方式一：迭代器 + auto（auto = 让编译器自动推断类型，省得写一长串）
for (auto it = v.begin(); it != v.end(); ++it)
    cout << *it << " ";          // 10 20 30

// 遍历方式二：范围 for，最常用
for (int x : v)
    cout << x << " ";            // 10 20 30
```

> 新手记两条就够：**① 遍历用范围 for；② 区间永远左闭右开。**

## 3. vector —— 动态数组（最常用的容器，没有之一）

你学过数组：`int a[100];`。vector 就是**会自动变长、还自带一堆功能的数组**。

```cpp
#include <bits/stdc++.h>
using namespace std;
int main() {
    vector<int> v;              // 空 vector
    vector<int> v2(10, 0);      // 10 个元素，都是 0

    int n; cin >> n;
    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        v.push_back(x);         // 在末尾加一个 x
    }

    cout << v.size() << "\n";   // 元素个数
    for (int i = 0; i < v.size(); i++) cout << v[i] << " ";  // 下标访问（和数组一样）
    for (int x : v) cout << x << " ";                        // 范围 for
    return 0;
}
```

### 常用操作速查（OI Wiki 序列式容器页的复杂度结论）

| 操作 | 含义 | 复杂度 |
|---|---|---|
| `push_back(x)` | 末尾加一个 x | O(1)（均摊） |
| `pop_back()` | 删掉末尾一个 | O(1) |
| `v[i]` | 下标访问 | O(1) |
| `front()` / `back()` | 第一个 / 最后一个元素 | O(1) |
| `size()` / `empty()` | 元素个数 / 是否为空 | O(1) |
| `clear()` | 清空 | O(n) |
| `insert(it, x)` / `erase(it)` | 中间插入 / 删除 | **O(n)**（要挪动后面所有元素） |

### 二维 vector（相当于 n 行 m 列的表格）

```cpp
vector<vector<int>> a(n, vector<int>(m, 0));   // n 行 m 列，全 0
a[i][j] = 5;
```

### 两个直觉

1. **末尾操作快，中间插删慢。** 所以日常就把它当"加强版数组"用：末尾 push_back，下标访问。
2. **`v[i]` 越界不会报错**（不像 Python 会提示），会读到乱七八糟的内存甚至直接崩。每次用下标前确认 `i < v.size()`。

> 进阶（现在可以跳过，以后回头看）：`resize(n)` 直接改大小；`reserve(n)` 提前预留容量，避免反复扩容——OI Wiki 的 vector 小节专门讲了 `size` 和 `capacity` 的区别，知道"push_back 时可能整体搬家扩容"这个现象即可。

**例题**：洛谷 **P1177【模板】排序**——读入 n 个数存进 vector，`sort` 后输出（sort 第二讲细讲，现在可以先抄一遍跑通）。

## 4. string —— 字符串

比你学过的 `char` 数组好用得多：自动管理长度，`+` 拼接、`==` 比较直接写。

```cpp
string s = "hello", t = " world";
string u = s + t;            // "hello world"
s += "!";                    // 追加
cout << s.size() << "\n";    // 长度
if (s == t) ...              // 直接比较内容，不用 strcmp
cout << s[0];                // 和数组一样按下标取字符
```

### 输入的两大坑（机试高频翻车点）

```cpp
string s;
cin >> s;            // 读到空白（空格/换行）就停 → 只能读一个"单词"
getline(cin, s);     // 读一整行（包含空格）
```

**经典坑**：先用 `cin` 读了数字，紧接着用 `getline`，会读到上一次残留的换行符，结果字符串是空的。解法：

```cpp
int n; cin >> n;
cin.ignore();        // 或 getchar(); —— 吃掉残留的换行
string s; getline(cin, s);
```

### 常用功能（先会用这三个）

```cpp
s.find("abc");        // 找子串，返回位置；找不到返回 string::npos（一个特殊值）
s.substr(2, 3);       // 从下标 2 开始截 3 个字符
to_string(123);       // 数字 → 字符串 "123"
stoi("42");           // 字符串 → 整数 42
```

### 删除字符（string 的删除三件套）

```cpp
string s = "hello world";

s.erase(2, 3);        // 从下标 2 开始删 3 个：删掉下标 2、3、4（'l','l','o'）→ "he world"（实测）
s.pop_back();         // 删末尾一个字符 → "hello worl"
s.clear();            // 清空 → ""（size 变 0）
```

常用套路（机试高频）：

```cpp
s.erase(s.begin() + 1);          // 删指定位置的单个字符（begin()+k 指向下标 k）
s.erase(remove(s.begin(), s.end(), ' '), s.end());  // 删掉所有空格（remove-erase 惯用法）
```

> 三个坑：
> 1. **`erase(pos, len)` 的第二个参数是"长度"不是"结束下标"**——和 `substr` 一样，`erase(2, 3)` 删的是下标 2、3、4 共 3 个，不是删到下标 3。
> 2. **`remove` 不会真的删**：它把目标字符挪到末尾并返回"新末尾"，必须再 `erase` 一刀才真正删掉（和 vector 的 remove-erase 一模一样，`vector删元素全攻略.md` 有详解）。
> 3. **下标越界是未定义行为**：`erase(pos, len)` 中 `pos > size()` 会抛异常，`pos == size()` 合法但啥也不删；`erase(begin()+k)` 要保证 `k < size()`。

**例题**：洛谷 **P1553 数字反转（升级版）**（字符串模拟，经典）；进阶 **P1308 统计单词数**（用 find）。

## 5. pair —— 把两个值打包成一个

```cpp
pair<int, int> p = {3, 5};   // 或 make_pair(3, 5)
cout << p.first << " " << p.second;   // 3 5
```

两个用途：① 函数想一次返回两个值；② 排序时"按分数排，同分按学号排"（把 `pair<int,int>` 存进 vector 直接 sort，**自动先比 first、再比 second**）。

## 6. stack —— 栈（后进先出）

想象一摞盘子：后放上去的，先被拿走。

```cpp
stack<int> st;
st.push(3); st.push(5);
cout << st.top();   // 5 —— 只看栈顶
st.pop();           // 弹掉栈顶 5（注意：pop 不返回任何值！）
st.empty(); st.size();
```

**经典应用：括号匹配。** 遇到 `(` 入栈，遇到 `)` 时若栈空则不合法，否则弹栈；最后栈必须为空。
**例题**：洛谷 **P1739 表达式括号匹配**；进阶 **P1449 后缀表达式**（栈的经典模拟）。

## 7. queue —— 队列（先进先出）

就是排队：先来的先被服务。

```cpp
queue<int> q;
q.push(1); q.push(2);
cout << q.front();   // 1 —— 队首
cout << q.back();    // 2 —— 队尾
q.pop();             // 弹出队首
q.empty(); q.size();
```

**例题**：洛谷 **P1996 约瑟夫问题**（经典模拟）、**P1540 机器翻译**（普及组真题）。
> queue 是以后学 BFS（图论搜索）的必备工具，这关必须过。

## 8. priority_queue —— 优先队列（自动取最值）

你可以理解成：一个箱子，每次放一个数进去，`top()` 永远是当前**最大**的数，`pop()` 弹掉最大的。底层是"堆"，但现在不用管原理。

```cpp
priority_queue<int> pq;           // 默认大根堆：top() 是最大值
pq.push(3); pq.push(7); pq.push(5);
cout << pq.top();   // 7
pq.pop();           // 弹掉 7

// 小根堆：top() 是最小值（写法要背下来）
priority_queue<int, vector<int>, greater<int>> pq_min;
```

注意：它**没有 `front`/`back`，只有 `top`**。

**经典应用**：反复"取出最小、放回新值"的贪心题。
**例题**：洛谷 **P3378【模板】堆**（小根堆）、**P1090 合并果子**（贪心 + 小根堆，必做）。

> 进阶（先跳过也行）：给结构体自定义优先级，第二讲末尾附了模板写法。

---

## 本讲小结

| 容器 | 一句话记忆 | 头文件里的位置 |
|---|---|---|
| `vector` | 自动变长的数组 | 序列式容器 |
| `string` | 高级版字符数组 | 独立一章 |
| `pair` | 两个值打包 | 独立一章 |
| `stack` | 后进先出，一摞盘子 | 容器适配器 |
| `queue` | 先进先出，排队 | 容器适配器 |
| `priority_queue` | 自动取最大/最小 | 容器适配器 |

**今天就能做的练习**：P1177、P1739、P1996、P3378、P1553。
第二讲继续：set / map / unordered_map 和算法库（sort、二分、去重、全排列）——那是机试真正的得分主力。
