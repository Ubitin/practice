# string 的 append() 详解：加强版 `+=`

> 定位：`<string>` 成员函数系列之三（前有 `string的substr详解`，姊妹篇 `string的insert/erase/replace` 可循同思路）。
> 关联：速查手册 §2.2 string、`字符串两兄弟-char数组与string对照`、STL 讲义-01 §string。
> 语言：C++11 起；示例均可直接编译验证（`#include <bits/stdc++.h>`）。

---

## 0. 一句话定位

`append()` 把**一段内容追加到当前字符串末尾**，返回 `*this` 引用（可链式）。
`string s;  s.append(t);` 与 `s += t;` **完全等价**——`operator+=` 底层就是调 `append`。
但 `append` 的重载比 `+=` 丰富得多，能处理"子串 / 前 n 个字符 / 重复字符 / 迭代器区间"四类 `+=` 做不到的追加。

## 1. 重载全家（按用途分 5 组）

```cpp
// ① 追加整个 string / C 字符串（与 += 等价）
s.append(t);            // t 是 string
s.append("hello");      // C 字符串，追加到 '\0'

// ② 追加 t 的"一段子串"（+= 做不到）
s.append(t, pos, len);  // 追加 t[pos .. pos+len)
//                       // 等价 s += t.substr(pos, len)，但零临时对象、更快

// ③ 追加 C 字符串的"前 n 个字符"（二进制安全，可含 '\0'）
char buf[] = "hello\0world";
s.append(buf, 5);       // 只追加 'h''e''l''l''o'，不会因 '\0' 提前停

// ④ 追加 n 个相同字符
s.append(5, '*');       // 追加 "*****"（等价 s += string(5,'*')，省一次临时构造）

// ⑤ 追加迭代器区间 / 初值列表（C++11）
vector<char> v = {'a','b','c'};
s.append(v.begin(), v.end());   // 把 char 容器内容整段追加
s.append({'x','y'});            // 初值列表版
```

> C++17 起另有 `string_view` 重载（`append(sv)`、`append(sv, pos, len)`），原理一致：把一个"可转 string_view 的东西"整段或其中一段追加进来。

## 2. append vs += 选型表

| 需求 | 推荐写法 | 备注 |
|---|---|---|
| 整体拼 string | `s += t` | 等价 `append(t)`，日常最常用 |
| 拼 C 字符串 | `s += "hi"` | 等价 `append("hi")` |
| 拼单个字符 | `s += 'x'` | **没有 `append('x')`**（见坑 1） |
| 拼 t 的子串 | `s.append(t, pos, len)` | `+=` 做不到 |
| 拼 n 个同字符 | `s.append(n, ch)` | `+=` 做不到 |
| 拼 char 数组一部分 | `s.append(buf + k, n)` | `+=` 做不到（`+=` 只能拼到 '\0'） |
| 拼一段迭代器内容 | `s.append(first, last)` | 配 vector<char> 等 |

## 3. 返回值 = *this（链式拼接）

```cpp
s.append("a").append(t).append(1, '!');   // 一口气拼三段，自左向右执行
```

## 4. 坑清单（必背）

1. **没有"单字符"版**：`s.append('x')` 编译错误（无 `append(char)` 重载，char 不能隐式转 `const char*`/`string`）。单字符请 `s += 'x'`，或写全 `s.append(1, 'x')`。
2. **两参版本第二个数是"长度"不是"次数"**：`s.append("abc", 2)` = 追加 "abc" 前 2 个字符 → `"ab"`。想重复追加：`s.append(2, 'x')`（2 个 x）。**参数顺序要看清**。
3. **pos 与 len 检查不对称**：`s.append(t, 100, 3)` 若 100 ≥ t.size() → 抛 `std::out_of_range`；`s.append(t, 1, 999)` len 超长 → 不报错，只追加到 t 结尾。记忆钩子：**pos 要合法（检查），len 超了截断（不检查）**——与 `substr(pos,len)` 边界规则完全一致。
4. **自追加别写 `s.append(s)`**：追加源是自身时，扩容可能使源内容失效 → 未定义行为。要"自己拼自己"用 `s += s;`（标准对 += 的自追加有保证），或先拷贝：`string tmp = s; s.append(tmp);`。
5. **append 只能到末尾**：插中间用 `s.insert(pos, ...)`；append 本质 = `insert(s.end(), ...)`。
6. **`append(t, pos)`（省略 len）**：追加 t 从 pos 到结尾的整段，是合法形态，别误以为少参数是错误。

## 5. 姊妹成员对照

| 需求 | 函数 | 一句差异 |
|---|---|---|
| 尾部追加 | `append` / `+=` | 本讲主题 |
| 中间插入 | `insert(pos, ...)` | 参数同为 (pos 或迭代器, 内容)，规则相似 |
| 删除 | `erase(pos, len)` / `erase(iter)` | 无返回值链式 |
| 替换 | `replace(pos, len, str)` | 先删后插的封装 |
| 取子串 | `substr(pos, len)` | 只读，返回新 string（append 子串版可避免这个临时对象） |

## 6. 习题

**E1** 指出错误并改正：`string s = "abc"; s.append('d');`

**E2** `char buf[] = "a\0b\0c";` 执行 `s.append(buf, 3)` 后，`s` 的内容与 `s.size()` 各是什么？

**E3** 补全：想把 `string t = "hello world"` 的第 6 位起 5 个字符（`"world"`）追加到 `s`，用哪一行？

**E4** 判断：`s.append(t, 3, 999)` 会抛异常吗？`s.append(t, 999, 3)` 呢？

---

## 答案与解析

**E1** → 编译错误：没有 `append(char)`。改正：`s += 'd';` 或 `s.append(1, 'd');`。

**E2** → `s` 内容为 `"a\0b"`（三个字符 a、'\0'、b），`s.size() == 3`。`append(buf, n)` 按长度 n 取前 n 个字符，**不**因中间的 '\0' 提前停——这正是它比 `s += buf` 安全的地方。

**E3** → `s.append(t, 6, 5);`（等价 `s += t.substr(6, 5);`，但零临时对象）。

**E4** → 第一句不抛：len=999 超长只截断到 t 结尾；第二句抛 `std::out_of_range`：pos=999 ≥ t.size()，pos 必须合法。

---

**一句话总结**：`append` = 加强版 `+=`，`append(t)` 等价 `+= t`，还能"追加子串 `append(t,pos,len)`、前 n 个字符 `append(buf,n)`、n 个同字符 `append(n,ch)`、迭代器区间"；**没有单字符版（用 `+=`）、pos 越界抛异常而 len 越界只截断、返回 `*this` 可链式**——把这四条记住，append 就算吃透了。

---

## 增补 · 2026-09-04：append vs push_back 对比

### 一句话差异

| | `push_back(ch)` | `append(...)` |
|---|---|---|
| 追加内容 | **只能 1 个字符** | 一串：字符串 / 子串 / n 个字符 / 迭代器区间 |
| 参数 | `char` | 各种重载 |
| 返回值 | `void` | `*this`（可链式） |
| 单字符场景 | ✅ 就是为它设计的 | ⚠️ 没有单字符版（要写 `append(1,ch)`） |

**本质**：`push_back` 是"容器接口"（vector 也有），每次塞**一个元素**；`append` 是"字符串专属"，一次追加**一段内容**。

### 代码对照

```cpp
string s = "AB";
s.push_back('C');      // "ABC"      一次一个字符
s += 'D';              // "ABCD"     += 也能拼单个字符
s.append("EF");        // "ABCDEF"   追加整串
s.append(2, '!');      // "ABCDEF!!" 追加 2 个 '!'
```

### 选型口诀

```
只加一个字符 → push_back(ch) 或 s += ch（首选）
加字符串/一段 → append(...) 或 s += "str"
要链式拼接   → append（返回 *this）
```

### 三个易混点

1. **'x' vs "x"**：`s.push_back('x')` ✅ / `s.push_back("x")` ❌（push_back 只收 char）；`s.append('x')` ❌（append 无单字符版）、`s.append("x")` ✅——两者对着收对方的类型就报错。
2. **链式**：`push_back` 返回 void 不能链；`append` 返回 `*this` 可以 `append("a").append("b")`。
3. **联想 vector**：push_back 与 vector 语义一致（尾部插一个元素）；append 是 string 特有，vector 里批量尾部插入用 `v.insert(v.end(), first, last)`。

### 姊妹函数 pop_back（C++11）

```cpp
s.push_back('d');   // 加末尾字符
s.pop_back();       // 删末尾一个字符（C++11 起）
```

**本增补一句话**：单字符用 push_back/+=，成段内容用 append/+=；push_back 收 char 不收串、返回 void，append 收串不收单字符、可链式——两者的"盲区"正好互补。
