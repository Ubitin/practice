# string 的 insert 详解 —— 插在 pos 前还是后？

> 问题：`s.insert(pos, n, ch)` 从下标 pos 开始插入，是插在该字符**之前**还是**之后**？
> 答案：**插在 pos 之前**。`pos` 是"插入位置"（insertion point），不是"参照字符"：
> 新内容占据从 `pos` 开始的位置，**原来在下标 pos 的字符以及它后面的一切整体后移**。

---

## 一、实测（g++ 16.2）

```
s = "abcdef";  s.insert(2, 3, 'X')   →  "abXXXcdef"     ← 原下标 2 的 'c' 被推到后面
s = "abcdef";  s.insert(0, 2, '[')   →  "[abcdef"       ← pos=0 → 插到最前面
s.insert(s.size(), 3, ']')           →  "[[abcdef]]]"   ← pos == size() 就是"末尾追加"
u = "hello";   u.insert(u.begin()+1,'A') → "hAello"     ← 迭代器版同理：插在该位置之前
v = "abc";     v.insert(1, "ZZZ")    →  "aZZZbc"        ← 插在下标 1 的 'b' 之前
```

记忆口诀：**`insert(pos, …)` = "让这一段落在 pos 上"**，原 pos 处字符往后让位。
想"插在某个字符**之后**"，就把 `pos` 加一：`s.insert(s.find('d') + 1, 1, '-')`。

---

## 二、常用重载（都在 `<string>`）

| 写法 | 含义 |
|---|---|
| `s.insert(pos, t)` | 在 pos 前插入整个字符串 `t` |
| `s.insert(pos, t, subpos, sublen)` | 插入 `t` 的子串 |
| `s.insert(pos, cnt, ch)` | 在 pos 前插入 **cnt 个**字符 `ch` |
| `s.insert(pos, cp)` / `(pos, cp, n)` | 插入 C 字符串（/ 其前 n 个字符） |
| `s.insert(it, ch)` | 迭代器版：在 `it` 前插入一个字符 |
| `s.insert(it, cnt, ch)` | 迭代器版：插入 cnt 个 ch |
| `s.insert(it, first, last)` | 插入别的迭代器区间（如 `s2.begin(), s2.end()`） |

**返回值**（C++11 起）：迭代器版返回"指向第一个被插入字符"的迭代器（实测 `u.insert(u.begin()+1,'A')` 返回的迭代器指向 `'A'`）；
下标版返回 `*this` 的引用（可链式调用）。

---

## 三、边界与坑

| 情形 | 行为 |
|---|---|
| `pos == s.size()` | **合法**，等于追加（实测 `"abc"` → `insert(3,1,'!')` → `"abc!"`） |
| `pos > s.size()` | **抛 `out_of_range`**（实测） |
| 迭代器版传入其它串的迭代器 | UB（迭代器必须属于本串） |
| 插入后原迭代器/引用 | **可能失效**（插入会引发扩容）——和 `vector` 一样，别在循环里用旧迭代器 |
| 循环里反复 `find` + `insert` | 每次 insert 是 O(n)，整体可能退化成 O(n²)，数据大时要另想办法（先拼接再输出） |

> 顺带对照：`erase(pos, cnt)` 也是"从 pos 开始、删 cnt 个"，即删除 **[pos, pos+cnt)**；
> 二者配合使用时位置语义一致：`pos` 都是**左端点**。

---

## 四、实战套路

```cpp
// ① 在某个字符前后插字符
size_t p = s.find('d');
if (p != string::npos) { s.insert(p, 1, '-'); }        // 在 'd' 前插
if (p != string::npos) { s.insert(p + 1, 1, '='); }    // 在 'd' 后插（注意 +1）

// ② 加前缀/后缀
s.insert(0, "0x");                                      // 前缀
s.insert(s.size(), 1, '.');                             // 后缀（或用 s += '.'）

// ③ 拼装含固定分隔符的串（比逐个 substr 更快：一次插入整段）
s.insert(pos, 3, '*');                                  // 插 3 个 '*'

// ④ 少用 insert 的场景：大量插中间 → 改成"读入到两个串再合并"/最后统一输出
```

**和 `+` / `+=` 的取舍**：
- 末尾追加 → `+=`（O(1) 均摊，最常用）；
- 指定位置插入 → `insert`（中间插入 O(n)，因为要搬后面的字符）；
- 大量位置构造 → 直接 `s = A + B + C`（编译器会优化，读起来也更清楚）。

---

## 五、自查

1. `"abcdef".insert(2, 1, 'X')` 的结果是什么？
2. 想在 `'d'` **后面**插入 `'-'`，`pos` 该怎么算？
3. `s.insert(s.size(), 1, '!')` 合法吗？等价于什么？
4. 为什么循环里"每次 insert 到串中间"可能很慢？

---

## 六、版本兼容性（`insert(pos, cnt, ch)` 从哪个 C++ 版本开始有？）

**结论：`basic_string& insert(size_type pos, size_type n, charT c)` 自 C++98 起就存在**，
C++98 / 03 / 11 / 14 / 17 / 20 / 23 **全部可用**，签名从未改变（返回 `*this` 的引用，可链式调用）。
它没有被弃用，也不受任何后续标准改动影响。

### 实测：同一段代码按 7 个标准编译（g++ 16.2）

```
g++ -std=<标准> insert_test.cpp        // 代码里就一句 s.insert(2, 3, 'X');
  c++98  通过 ✓      c++11  通过 ✓      c++17  通过 ✓      c++23  通过 ✓
  c++03  通过 ✓      c++14  通过 ✓      c++20  通过 ✓
```

### 其它 insert 重载的版本差异（与本题无关，但容易踩）

| 重载 | 出现版本 | 备注 |
|---|---|---|
| `insert(pos, n, ch)` ← **本次问的** | **C++98** | 全版本一致 |
| `insert(pos, str)` / `insert(pos, cp)` / `insert(pos, cp, n)` | C++98 | |
| `insert(pos, str, subpos, sublen)` | C++98（**C++14 起 `sublen` 有默认值 `npos`**） | C++14 之后可以只写 `subpos` |
| `insert(iterator, charT)` | C++98 为 `void`；**C++11 起改为 `iterator insert(const_iterator, charT)`** | 入参由 `iterator` 变 `const_iterator`，并开始返回迭代器 |
| `insert(const_iterator, initializer_list<charT>)` | **C++11** 新增 | 可 `s.insert(s.begin(), {'a','b'})` |
| `insert(pos, string_view)` 系列 | **C++17** 新增 | 与 `string_view` 互操作 |

### 实测验证签名差异（用"成员函数指针"探针）

```
探针：std::string::iterator (std::string::*q)(std::string::const_iterator, char) = &std::string::insert;
  -std=c++98  不可编译 ✗      ← C++98 的迭代器版参数是 iterator（且标准规定返回 void）
  -std=c++11  可编译 ✓        ← C++11 起是 const_iterator 入参、返回 iterator
  -std=c++17  可编译 ✓
```

> 注意一个"实现放宽"现象：当前 libstdc++ 在 `-std=c++98` 下，迭代器版**也**返回迭代器（严格按 C++98 应为 `void`）。
> 所以**不要用"编译能否通过"反推标准版本**，要看标准条文。

### 实践建议

- 洛谷 / Codeforces / 各 OJ 默认 C++14/17/20 → `insert` 任何重载都可放心用；
- 只有在极老的 C++98 编译器上才需注意"迭代器版没有返回值"这条（`insert(pos, cnt, ch)` 不受影响）；
- 报错信息若提示 `insert` 相关歧义，多半是**重载**选错（例如把 `char` 变量当 `n` 传、或 `int` 与 `size_type` 混用），而不是版本问题。

### 补充自查

5. 在 `-std=c++98` 下 `insert(pos, 3, 'x')` 能编译吗？（能 —— C++98 起就有）
6. `s.insert(s.begin(), 'x')` 的返回值在 C++98 与 C++11 下有什么差别？（C++98 标准是 void；C++11 起返回指向首个插入字符的迭代器）

答案：1. `"abXcdef"`（插在下标 2 的 `'c'` 之前）。2. `pos = s.find('d') + 1`。3. 合法，等价于 `s += '!'`（末尾追加）。4. 每次插入都要把插入点之后的字符整体后移，单次 O(n)，循环 k 次就可能是 O(nk) 甚至 O(n²)。
