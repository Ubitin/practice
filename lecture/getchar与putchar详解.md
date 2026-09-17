# getchar 与 putchar 详解：C 的"单字符"输入输出

> 定位：输入输出系列拼图之一（关联 `输入输出全攻略`、`输出格式控制`）。
> 语言：C/C++ 通用（`#include <cstdio>`；`bits/stdc++.h` 已包含）。
> 核心结论全部基于 C 标准库语义，可在 g++ 上直接编译验证。

## 0. 一句话定位

`getchar()` 从标准输入**读一个字符**；`putchar(c)` 向标准输出**写一个字符**。
它们是 C 标准库最底层的字符级 IO——一次只处理一个字符（字节）。

## 1. 原型与返回值（重点：为什么返回 int）

```cpp
int getchar(void);   // 读到文件尾或出错返回 EOF（通常 = -1）
int putchar(int c);  // 写 c（按 unsigned char 写）；成功返回该字符，失败返回 EOF
```

**必考概念：getchar 返回 int，不是 char。为什么？**

- 输入流中的字节共有 256 种可能值（0~255）；
- 但我们还需要一个额外的"读完了/出错了"哨兵 → EOF；
- `char` 只有 256 种取值，**全部被字符占用，没有多余编码代表 EOF**；
- 所以标准库用 `int`（装得下 0~255 + EOF 哨兵），读到的字节转成 int 返回。

**用 `char` 接返回值必出 bug**（详见 §5 坑 2）：
- `char` 为无符号时：EOF(-1) 存进去变成 255，`c != EOF` 永不成立 → **死循环**；
- `char` 为有符号时：内容里合法的字节 0xFF(255) 变成 -1 → **被误判为 EOF，数据提前丢**。

## 2. 最关键的性格：getchar() 不跳过空白

| 读法 | 跳过前导空白？ | 说明 |
|---|---|---|
| `scanf("%d")` / `cin >>` | ✅ 跳过 | 读数字/词，遇空白停 |
| `scanf("%c")` | ❌ 不跳 | 见什么读什么 |
| `getchar()` | ❌ 不跳 | 见什么读什么（空格、`'\n'`、Tab 都算字符） |

这正是它常被用来**吸收残留换行**的原因：

```cpp
int n;
scanf("%d", &n);   // 输入 "42\n"：%d 读到 42 停下，'\n' 留在缓冲区
getchar();         // ✅ 把这个 '\n' 吃掉（等价于 C++ 的 cin.ignore()）
char ch = getchar();   // 这时读到的才是真正的下一个字符
```

## 3. 三大经典用法

### 用法 1：吸收残留换行（最高频）
见 §2 示例。C++ 对应写法：`cin >> n;` 后 `cin.ignore();`。

### 用法 2：逐字符处理到 EOF（机试/文件处理）

```cpp
#include <cstdio>
int main() {
    int c;
    while ((c = getchar()) != EOF) {   // 括号不能省！见坑 1
        putchar(c);                    // 逐字符原样输出（等价 cat）
    }
    return 0;
}
```

### 用法 3：字符快读模板（提速用，机试可背）

```cpp
int read() {
    int x = 0, f = 1; char ch = getchar();
    while (ch < '0' || ch > '9') { if (ch == '-') f = -1; ch = getchar(); }
    while (ch >= '0' && ch <= '9') { x = x * 10 + ch - '0'; ch = getchar(); }
    return x * f;
}
```

思路：跳过非数字（顺带识别负号）→ 连续收集数字 → 乘以符号 f。比 scanf 再快一档（已收入速查手册 §1.4）。

## 4. putchar 用法

```cpp
putchar('A');      // 输出字符 A
putchar('\n');     // 输出换行（比 printf("\n") 轻）
putchar(c);        // c 为 int/char 均可：按 unsigned char 输出
```

用途：配 getchar 逐字符原样输出；手写"快写"（把数字一位位拆出来输出）也靠它。

## 5. 常见坑清单

1. **`while (c = getchar() != EOF)` 少括号** → `!=` 优先级高于 `=`，等价于 `c = (getchar() != EOF)`，c 只会是 0/1，逻辑全错。必须 `(c = getchar()) != EOF`。
2. **用 `char c` 接 getchar** → 与 EOF 混淆（§1）：无符号死循环 / 有符号丢 0xFF。一律 `int c;`。
3. **忘掉 getchar 不跳空白** → 想"跳过行首空格再读字符"会失败；跳过空白请用 `scanf(" %c")`（`%c` 前加一个空格）。
4. **读走的字符回不来** → 输入是"流"，吃掉的字符无法放回（那是 `ungetc` 的职责，此处不展开）。
5. **`putchar(c)` 想一次输出多位数字** → 只能输出一个字符；多位要拆位或用 printf/cout。
6. **终端交互是"行缓冲"**：getchar() 要等你按回车才拿到字符。OJ/文件输入无此现象，不用管。

## 6. C++ 对照

| 需求 | C | C++ |
|---|---|---|
| 读一个字符 | `getchar()` | `cin.get()`（或读进 string 取 `s[0]`） |
| 吃残留换行 | `getchar()` | `cin.ignore()` |
| 写一个字符 | `putchar(c)` | `cout << c` |
| 多组到 EOF | `while ((c = getchar()) != EOF)` | `while (getline(cin, s))` |

C++ 里字符级 IO 用得少（有 string），但"吃换行"和"逐字符处理"两个场景绕不开 getchar。

## 7. 习题（可直接编译验证）

**E1** 输入恰好是 `ab` + 回车（即 `"ab\n"`），依次执行：
```cpp
int a = getchar(), b = getchar(), c = getchar();
```
问 a、b、c 分别等于什么（用字符与数值回答）？

**E2** 找 bug：下面代码在什么情况下死循环、什么情况下提前停？
```cpp
char c;
while ((c = getchar()) != EOF) putchar(c);
```

**E3** 用 getchar 写：读到 EOF，统计并输出小写字母个数（输入只含字母、空格、换行）。
提示：`if (c >= 'a' && c <= 'z') cnt++;`

**E4** `scanf("%d", &n);` 读入 `42` 后，紧接 `getchar()` 返回什么？

---

## 答案与解析

**E1** → `a = 'a'(97)`，`b = 'b'(98)`，`c = '\n'(10)`。getchar 见什么读什么，回车也是字符。

**E2** → 用 `char` 接 EOF 是错的（§1）：若 char 无符号，EOF(-1) 存成 255，永远不等于 -1 → **死循环**；若 char 有符号，内容中出现字节 0xFF 会被误判成 EOF → **提前停**。修复：`int c;`。

**E3** →
```cpp
int c, cnt = 0;
while ((c = getchar()) != EOF)
    if (c >= 'a' && c <= 'z') cnt++;
printf("%d\n", cnt);
```

**E4** → 返回 `'\n'`（10）。`%d` 读到数字结束停下，残留的换行被这次 getchar 吃掉（这正是"吸收换行"的由来）。

---

**一句话总结**：getchar/putchar 是"一次一个字符"的 C 级 IO；**返回 int 是为了装下 256 种字符 + EOF 哨兵**；getchar **不跳空白**所以能"吃换行"；逐字符读到文件尾用 `while ((c = getchar()) != EOF)`——**c 用 int、括号不能省**。
