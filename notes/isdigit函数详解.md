# isdigit 详解 —— 以及 ctype 家族速查

> 一句话：`isdigit(c)` 判断**单个字符** `c` 是不是十进制数字字符（`'0'`~`'9'`）。
> 头文件 `<cctype>`（C 里是 `<ctype.h>`；提交时 `bits/stdc++.h` 已包含）。
> 本文所有实测结果来自 g++ 16.2（Windows/w64devkit，glibc 风格实现）。
> 配套：`字符串统计详解-count与桶计数`（count_if + isdigit 实战）、`getchar与putchar详解`（字符快读）、`ECNA2023-I-ISBN转换-排错与标准规则`（'X' 校验位必须单独判）。

---

## 一、原型与语义

```cpp
int isdigit(int c);      // 参数是 int，不是 char！
```

- **返回非 0**：`c` 是 `'0'`~`'9'` 之一（等价于 `c >= '0' && c <= '9'`）；
- **返回 0**：其它任何字符。

用法就是当条件用：

```cpp
char c;
if (isdigit(c)) { int v = c - '0'; ... }   // 注意 c 的类型问题，见坑 ①
```

---

## 二、三个必须记住的细节（都实测过）

### 坑 ①：参数必须是 `unsigned char` 的值或 `EOF`——直接传 `char` 可能 UB

`isdigit` 的参数是 `int`，标准要求传入的值要么可表示为 `unsigned char`（0~255），要么是 `EOF`。
而 `char` 在多数平台上**是有符号的**，遇到非 ASCII 字节（如 UTF-8 中文的某个字节 `0xB2`）会是**负数**，传进去就是**未定义行为**。

实测：

```
(char)0xB2 = -78
isdigit(neg)                = 0   ← 传负值是 UB（这里"碰巧"返回 0，不代表安全）
isdigit((unsigned char)neg) = 0   ← 正确写法
```

**正确姿势**：`isdigit((unsigned char)c)`，或者干脆用 `for (char ch : s)` 时写 `isdigit((unsigned char)ch)`。

### 坑 ②：返回值是"非 0 即真"，标准**不保证等于 1**

```
isdigit('5') = 1        isdigit('X') = 0        (isdigit('5')==1)? 是
```

本机恰好返回 1，但不同 C 运行库可能返回别的非 0 值（例如某个位标志）。
所以**不要写** `if (isdigit(c) == 1)`，要写 `if (isdigit(c))` 或 `if (isdigit(c) != 0)`。

### 坑 ③：它只认 ASCII 数字，且只判"一个字符"

实测各字符：

| 字符 | `isdigit` | 手写 `'0'<=c<='9'` |
|---|---|---|
| `'5'` `'0'` `'9'` | 1 | 1 |
| `'X'` `'x'` | 0 | 0 |
| `'-'` `.` `' '` `'\t'` | 0 | 0 |
| `'a'` `'A'` | 0 | 0 |

要点：
- `'X'`、`'-'`、`'.'`、空格、Tab **都不是**数字 → 你 ECNA 那道 ISBN 题里，末位校验位可能是 `X`，**必须单独判断**，不能只靠 isdigit；
- 全角数字 `'５'`（U+FF10）在 UTF-8 里是多字节、单字节值为负 → `isdigit` 对它是 0（且直接传还会踩坑 ①）；
- `isdigit` 判断的是**单个字符**，想判断"整个字符串是不是数字"要用 `strtol`/手写循环/`all_of`。

---

## 三、和手写判断等价吗？

在 ASCII 环境（OJ 全是）下：`isdigit(c)` ≡ `c >= '0' && c <= '9'`。

| | `isdigit((unsigned char)c)` | `c >= '0' && c <= '9'` |
|---|---|---|
| 语义 | 相同 | 相同 |
| 负数 `char` | 必须先 cast，否则 UB | 直接用也安全（比较运算不会越界） |
| 速度 | 函数调用/查表 | 更快（可内联、无调用） |
| 可读性 | 意图明确 | 也很清楚 |

**竞赛建议**：字符处理用 `isdigit((unsigned char)c)`；对性能敏感的地方（如快读）直接手写比较。

**配套常识**：只有确认了是数字字符，`c - '0'` 才是"字符 → 数值"的正确转换。

---

## 四、ctype 家族速查（同头文件 `<cctype>`，参数规则同上）

| 函数 | 判断/作用 | 备注 |
|---|---|---|
| `isdigit(c)` | `'0'`~`'9'` | 十进制数字 |
| `isxdigit(c)` | 十六进制数字 | `0-9a-fA-F` |
| `isalpha(c)` | 字母 | `a-z`、`A-Z` |
| `isalnum(c)` | 字母或数字 | |
| `islower(c)` / `isupper(c)` | 小写 / 大写字母 | |
| `isspace(c)` | 空白 | 空格、`\t`、`\n`、`\v`、`\f`、`\r` |
| `ispunct(c)` | 标点符号 | |
| `isprint(c)` / `isgraph(c)` | 可打印 / 可显示（不含空格） | |
| `iscntrl(c)` | 控制字符 | |
| `isblank(c)` | 空格或 Tab | C++11 |
| `tolower(c)` / `toupper(c)` | 转小写 / 大写 | **非字母原样返回**，也要 cast |

---

## 五、典型用法

```cpp
// ① 统计字符串里的数字字符个数
int cnt = count_if(s.begin(), s.end(), [](char c){ return isdigit((unsigned char)c); });

// ② 字符快读（配合 getchar 的经典写法）
int read(){
    int x = 0, f = 1; char ch = getchar();
    while (!isdigit((unsigned char)ch) && ch != '-') ch = getchar();   // 跳过非数字
    if (ch == '-') { f = -1; ch = getchar(); }
    while (isdigit((unsigned char)ch)) { x = x * 10 + (ch - '0'); ch = getchar(); }
    return x * f;
}

// ③ 解析一行里的数字（isbn / 时间 / 坐标题通用）
for (char c : line) if (isdigit((unsigned char)c)) vec.push_back(c - '0');
```

---

## 六、自查

1. `isdigit('X')`、`isdigit('-')`、`isdigit('.')` 分别是多少？
2. 为什么 `isdigit(c)` 里的 `c` 是 `char` 时可能有风险？正确写法？
3. `if (isdigit(c) == 1)` 这种写法有什么问题？
4. 想判断"字符串 `"123"` 是不是合法整数"，能用 isdigit 一次搞定吗？

答案：1. 全为 0（都不是十进制数字字符）。2. `char` 可能是有符号的，负数传入违反标准（UB）；写 `isdigit((unsigned char)c)`。3. 标准只保证"非 0 表示真"，不保证返回值恰好是 1，换运行库可能出错；应写 `if (isdigit(c))`。4. 不能；isdigit 只作用于单个字符，要对每个字符判断（并考虑符号、空串、前导 0 等规则），或用 `strtol` 这类整体解析函数。
