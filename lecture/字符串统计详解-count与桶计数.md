# 字符串统计详解：count / 子串计数 / 桶 / 前缀频次

> 前置：`STL讲义-01` §4（string 基础）、`string的substr详解`（切子串）、`计数排序-为什么是O(m+n)`（桶思想）、`前缀和与二维前缀和详解`（第 4 节依据）、`机试代码模板速查手册` §2.2 / §3.9。
> 演示程序：`test_\30.cpp`（全部结论已用 g++ 16.2 / C++17 / -O2 实测，原始输出见文末附录）。

---

## 0. 一句话本质

"统计字符串里某字符出现几次"有**四种问法**，对应四种不同量级的做法——**别拿最弱的那一种硬扛所有情况**：

| 问题 | 做法 | 复杂度 | 关键 API |
|---|---|---|---|
| **一个字符**出现几次 | `count(s.begin(), s.end(), ch)` | O(n) 走一遍 | `<algorithm>` |
| **一个子串**出现几次 | `find` 循环 | O(n·m)（朴素） | `s.find(t, pos)` |
| **每个字符各**几次 | 桶累计 `cnt[s[i]-'a']++` | O(n) 走一遍 | 数组/`array` |
| 反复问**区间内**某字符几次 | 前缀频次 `pre[i][c]` | O(26n) 预处理 + **O(1)**/次 | 前缀和思想 |

判断依据就一句话：**问一次 → 现算（count）；问很多次 → 预处理（桶 / 前缀频次）。**

## 1. count / count_if：最常用的那一个

```cpp
#include <algorithm>                      // 只包 <iostream>/<string> 时这行不能省
string s = "Banana 2024, banana!";

int a = count(s.begin(), s.end(), 'a');            // 6  统计"值相等"的字符
int d = count_if(s.begin(), s.end(),               // 4  统计"满足条件"的字符
                 [](char c){ return isdigit((unsigned char)c); });
```

**实测**（`test_\30.cpp` 输出）：

| 表达式 | 结果 |
|---|---|
| `count(s.begin(), s.end(), 'a')` | 6 |
| `count(..., 'B')` / `count(..., 'b')` | 1 / 1（**区分大小写**） |
| `count_if` + `isdigit` | 4 |
| `count_if` + `isalpha` | 12 |
| `count_if` + `isupper` | 1 |
| `count_if` + `!isspace` | 18 |

**要点**：
1. **返回类型是迭代器的 `difference_type`**，实测 `sizeof(...) == 8`、`typeid(...).name() == 'x'`（即 `long long`）；赋给 `int` 一般没问题，求稳用 `auto` 或 `long long`。
2. **`count` 一定走完全程**，不能提前退出——只想判断"在不在"用 `s.find(ch) != string::npos` 更快。
3. **区分大小写**；要忽略大小写就 `count_if` + `tolower`。
4. C++20 可写 `ranges::count(s, 'a')`，省掉两个迭代器。
5. 手写循环与 `count` 等价（实测都是 6），不会更慢：
   ```cpp
   int c = 0; for (char ch : s) if (ch == 'a') ++c;
   ```

## 2. 子串出现次数：**没有现成函数**，靠 `find` 循环

```cpp
// 不重叠计数：每次从 p + sub.size() 继续
int c = 0;
for (size_t p = s.find(sub); p != string::npos; p = s.find(sub, p + sub.size())) ++c;

// 允许重叠：步长改成 1
for (size_t p = s.find(sub); p != string::npos; p = s.find(sub, p + 1)) ++c;
```

**实测**：

| 文本 / 子串 | 不重叠 | 允许重叠 |
|---|---|---|
| `"aaaa"` / `"aa"` | 2 | 3 |
| `"abababa"` / `"aba"` | 2 | 3 |
| `"aaaa"` / `"b"` | 0（`find` 返回 `npos`，**不会死循环**） | 0 |

**要点**：
- `p` 必须用 **`size_t`**（`npos` 是 `size_t` 的最大值，用 `int` 接会踩 `size_t详解` 里的回绕坑）；
- 循环变量写成 `p = s.find(sub, p + step)` 是**"从上次命中处继续"**的标准姿势，别写 `p++` 从头再找（那会变成 O(n²)）；
- 数据量大（n、m 都上万）且要严格最坏复杂度时，才考虑 KMP；普及组/机试绝大多数情况 `find` 循环够用。

## 3. 桶计数：统计"每个字符各几次"

```cpp
int cnt[26] = {0};
for (char ch : s) if (isalpha((unsigned char)ch)) cnt[tolower(ch) - 'a']++;
```

**实测**：`"Banana 2024, banana!"` → `a=6 b=2 n=4`；与"对 26 个字母各调一次 `count_if`"的结果**逐一核对完全一致**。

**为什么要用桶，而不是循环 26 次 `count`？**

实测同一份 **n = 10⁶** 的随机小写串：

| 做法 | 耗时 | 遍历次数 |
|---|---|---|
| 26 次 `count` | **3.65 ms** | 26 遍 |
| 桶计数一遍 | **0.48 ms** | 1 遍 |

比值 ≈ **7.6 倍**。注意这里**不是 26 倍**——`count` 很可能被编译器优化成了 `memchr` 式的向量化扫描，单遍很快；但 **O(26n) 终究是 O(n) 的 26 倍常数**，数据再大或字母表更大（ASCII 128、中文 Unicode）时差距会继续拉大。**一次遍历能拿全，就别遍历 26 次。**

## 4. 前缀频次：反复问"区间里某字符几次"

```cpp
string t = "abacabadabacaba";
int n = t.size();
vector<array<int,26>> pre(n + 1);
pre[0].fill(0);
for (int i = 0; i < n; ++i) {
    pre[i + 1] = pre[i];          // 先继承上一格（26 个字符的累计）
    pre[i + 1][t[i] - 'a']++;     // 再把当前位置的字符 +1
}
// 询问：1-based 闭区间 [l, r] 里字符 ch 出现几次 —— O(1)
int c = pre[r][ch - 'a'] - pre[l - 1][ch - 'a'];
```

**实测**：用 5 组区间 × 4 个字符 = **20 次询问**与"用 `count` 逐段现算"的结果**全部一致**（`t = "abacabadabacaba"`，`[1,4]` 里 `'a'` 有 2 个）。

**要点**：
- 本质就是 `前缀和与二维前缀和详解` 的"前缀和"思想，只是把"和"换成"26 个字符的计数向量"；
- **内存要估**：`(n+1) × 26 × 4 B` → n = 10⁵ 约 **10 MB**（可用）；n = 10⁶ 就是 **104 MB**（会 MLE，得改用离线做法或只对出现的字符建前缀数组）；
- 一次询问 O(1)，预处理 O(26n)：**q 次询问 ≥ 26 时就比每次 `count`（O(qn)）划算**。

## 5. 常见坑

1. **类型不匹配**：`vector<int>` 上写字符字面量，比较的其实是 ASCII 值。实测 `count(vector<int>{97,98,97}.begin(), ..., 'a') == 2` —— `'a'` 被当成 **97**，能在 `vector<int>` 里"命中"，纯属巧合式的正确/错误。
2. **靠 `count` 判存在**：`count(...) > 0` 要走完全串；判存在用 `s.find(ch) != string::npos`。
3. **`isdigit`/`isalpha`/`tolower` 直接传 `char`**：`char` 可能是负数（如 `0xFF`），标准要求参数是**可表示为 `unsigned char` 的值**或 `EOF`，所以先 `(unsigned char)c` 再传（演示程序里就是这么写的）。
4. **对每个字符各调一次 `count`**：O(σn) 常数，正是第 3 节要避免的写法。
5. **`s.find` 的返回值用 `int` 接**：`npos` 是 `size_t` 最大值，`int` 接会变成 −1 或回绕，见 `size_t详解`。
6. **本机环境附注（实测踩坑）**：本机 g++（w64devkit 16.2，zh-CN / GBK 936）下，**字符串字面量里混排中文标点与 ASCII 时可能报出莫名其妙的语法错误**（如 `expected ';' before ')' token`）——例如 `"\n（与 26 次 count 对照："` 会失败，而结构相近的短串却正常。**中文写在注释里不受影响**（平时的代码能编过就是这个原因）。稳妥做法：
   - 演示程序/输出标签**只用 ASCII**；中文放注释；
   - 真要输出中文，把源文件另存为 **GBK**，或用 `\uXXXX` 转义 / `u8"..."`；
   - 这类报错**先怀疑编码**，别去改语法。

## 6. 习题

**E1** 读入一行（可能含空格），输出其中英文字母的个数（不含空格/数字/标点）。

**E2** 补全：统计 `string s` 中 `'x'` 的出现次数：
```cpp
int c = _______(s.begin(), s.end(), 'x');
```

**E3** 判断对错：
```
A. count 的返回值类型是 int
B. count 找到第一个匹配后就会提前结束
C. count_if 可以用 lambda 传条件
D. 统计 26 个字母频率，循环 26 次 count 是最优写法
```

**E4** 写代码：统计 `"abababa"` 中 `"aba"` 的出现次数，**允许重叠**。

**E5** 选择：要回答 **10⁵ 次**"`t[l..r]` 中有几个 `'a'`"的询问，用哪种做法？
```
A. 每次询问都 count(t.begin()+l-1, t.begin()+r, 'a')
B. 建前缀频次 pre[i][c]，询问时相减
C. 桶计数 cnt['a']
```

---

## 答案与解析

**E1** →
```cpp
string s; getline(cin, s);
cout << count_if(s.begin(), s.end(),
                 [](char c){ return isalpha((unsigned char)c); }) << "\n";
```

**E2** → `count`。

**E3** → **B、D 错，C 对，A 不严谨**。`count` 返回 `difference_type`（实测 8 字节，本机 `typeid` 为 `x` 即 `long long`），不是 `int`；`count` 必须走完全程才能给出总数；26 次 `count` 是 O(26n)，实测比一遍桶计数慢约 **7.6 倍**。

**E4** →
```cpp
string t = "abababa", sub = "aba"; int c = 0;
for (size_t p = t.find(sub); p != string::npos; p = t.find(sub, p + 1)) ++c;   // c = 3
```
（若改成 `p + sub.size()` 则不重叠计数，结果是 2 —— 两种口径都常见，看清题目要求。）

**E5** → **B**。A 是 O(qn) = 10⁵ × n，必超时；C 给不出"区间"信息；前缀频次 O(26n) 预处理后每次询问 O(1)，20 次抽样已与 `count` 逐段核对一致。

---

**一句话总结**：**一个字符用 `count`、一个子串用 `find` 循环、所有字符用桶、区间反复问用前缀频次**；`count` 是"问一次"的工具（O(n)、不能提前退出、区分大小写），一旦要"问很多次"就必须先预处理——这就是桶（O(n) 拿全部）与前缀频次（O(26n) 换 O(1) 查询）存在的意义。

---

## 附录：`test_\30.cpp` 原始输出（g++ 16.2 / C++17 / -O2）

```
=== (1) count / count_if ===
s = "Banana 2024, banana!"
count(s,'a')      = 6
count(s,'B')      = 1
count(s,'b')      = 1
count_if digit    = 4
count_if alpha    = 12
count_if upper    = 1
count_if !space   = 18

=== (2) manual loop & return type ===
manual loop       = 6  (same as count)
sizeof(count ret) = 8 bytes, typeid = x

=== (3) substring count ===
"aaaa" / "aa"    : no-overlap = 2, overlap = 3
"abababa" / "aba": no-overlap = 2, overlap = 3
"aaaa" / "b"     : no-overlap = 0  (find returns npos -> 0, no endless loop)

=== (4) bucket count (one pass) ===
freq: a=6 b=2 n=4
cross-check with 26x count_if: identical

=== (5) prefix frequency ===
t = abacabadabacaba
count of 'a' in [1,4] = 2
20 range queries vs count(): all identical

=== (6) type mismatch trap ===
count({97,98,97}, 'a') = 2  <- 'a' is treated as 97

=== (7) performance, n = 1e6 ===
26 x count   : 3.654 ms (sum = 1000000)
one-pass bucket: 0.479 ms (sum = 1000000)
ratio approx : 7.62839 x
```

> 复现：`g++ -std=c++17 -O2 -o test_\30.exe test_\30.cpp && test_\30.exe`（计时数值随机器浮动，量级与结论不变）。
