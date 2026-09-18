# fgets / sscanf / sprintf 详解：C 的"字符串读写 + 格式化"三件套

> 定位：三个函数都围绕 **C 字符串（char 数组）**，构成"读一行 → 解析字段 → 拼装输出"的完整闭环。
> 前置：`char数组详解`（C 字符串本质）、`输入输出全攻略`（scanf 残留换行/多组输入）、`文件操作讲义`（FILE* 与 freopen）。
> 语言：C/C++ 通用（`<cstdio>` / `<stdio.h>`，`bits/stdc++.h` 已包含）。

## 0. 一句话定位

| 函数 | 干什么 | 类比 |
|---|---|---|
| `fgets()` | 从**流/文件**读**一行**到 char 数组（带长度上限，安全） | 安全的 `gets` |
| `sscanf()` | 从**字符串**按格式**解析字段** | `scanf` 的"字符串版" |
| `sprintf()` | 把数据按格式**写进字符串** | `printf` 的"字符串版" |

## 1. fgets —— 读一行（安全版）

```c
char *fgets(char *str, int n, FILE *stream);
```

**行为**：从 `stream` 读最多 **n-1** 个字符存入 `str`，遇 `'\n'` 或 EOF 停止；**会保留 `'\n'`**（若读到）；末尾自动补 `'\0'`；成功返回 `str`，读到文件尾/出错返回 `NULL`。

```c
char buf[256];
fgets(buf, sizeof(buf), stdin);   // 从标准输入读一行，最多 255 个字符
```

**关键理解 —— `n` 是"缓冲区总大小"**：最多读 `n-1` 个字符 + 1 个 `'\0'`，所以写 `sizeof(buf)` 永不越界。别把 n 当成"要读的字符数"。

**⚠️ 它会保留 `'\n'`**，常需去掉：

```c
fgets(buf, sizeof(buf), stdin);
buf[strcspn(buf, "\n")] = 0;        // 把第一个 '\n' 替换成 '\0'（推荐，一行搞定）
// 或：int len = strlen(buf); if (len && buf[len-1]=='\n') buf[len-1] = '\0';
```

**vs `gets`**：`gets` 无长度限制，必然缓冲区溢出（已废弃）；`fgets` 必须给 n，安全。

**多组读到 EOF**：

```c
char buf[256];
while (fgets(buf, sizeof(buf), stdin)) {   // 返回 NULL 即 EOF
    /* 逐行处理 */
}
```

## 2. sscanf —— 从字符串解析

```c
int sscanf(const char *str, const char *format, ...);
```

从 `str`（不是 stdin）按格式解析，返回**成功解析的变量个数**；到字符串结尾返回 `EOF`。`&` 规则和 scanf 一致（**只有数组名不用 &**）。

把一行拆成字段：

```c
char line[] = "2024 张三 89.5";
int year; char name[20]; double score;
sscanf(line, "%d %s %lf", &year, name, &score);   // name 是数组名，不用 &
```

**最常用组合：fgets 读整行，再用 sscanf 解析**（"行处理"的标准姿势）：

```c
char line[100];
while (fgets(line, sizeof(line), stdin)) {
    int a, b;
    if (sscanf(line, "%d%d", &a, &b) == 2) {   // 判返回值 == 期望字段数！
        /* 成功才处理，避免用到垃圾值 */
    }
}
```

⚠️ **`%s` 读到空白就停**。要读含空格的段（如"带空格姓名"）用 `%[^\n]`：

```c
sscanf(line, "%d %[^\n]", &id, name);   // %[^\n] 读到换行为止（含空格）
```

## 3. sprintf / snprintf —— 拼装字符串

```c
int sprintf(char *str, const char *format, ...);
```

把格式化结果写进 `str`，返回**写出的字符数（不含 '\0'）**。**⚠️ 不检查长度，易缓冲区溢出** → 首选 `snprintf`。

```c
char buf[100];
int a = 42; double pi = 3.14159;
sprintf(buf, "a=%d, pi=%.2f", a, pi);   // buf = "a=42, pi=3.14"
```

**安全版 `snprintf`**：

```c
int snprintf(char *str, size_t size, const char *format, ...);
// size 是"缓冲区总大小（含 '\0'）"，最多写 size-1 个字符
char buf[100];
snprintf(buf, sizeof(buf), "a=%d", a);   // 安全，绝不越界
```

适用：把多个值按格式拼成一行字符串、拼接表头/输出行。简单拼接用 `strcat` 即可，不必 sprintf。

## 4. 三者串起来（完整例子）

```c
#include <stdio.h>
#include <string.h>
int main() {
    char line[256];
    fgets(line, sizeof(line), stdin);        // ① 读一整行
    line[strcspn(line, "\n")] = 0;           //   去换行

    int id; double score; char name[20];
    sscanf(line, "%d %s %lf", &id, name, &score);   // ② 拆字段

    char out[256];
    sprintf(out, "id=%d name=%s score=%.1f", id, name, score);  // ③ 拼装
    puts(out);
    return 0;
}
```

## 5. 坑清单

1. **fgets 保留 `'\n'`** → 需要时去掉（`strcspn(buf,"\n")=0` 或长度判断）。
2. **fgets 的 `n` 是缓冲区总大小**（最多读 n-1 + '\0'），别理解成"要读的字符数"。
3. **sprintf 不检查长度** → 用 `snprintf`，写 `sizeof(buf)`。
4. **sscanf 不判返回值** → 解析失败时变量是垃圾值。
5. **sscanf 忘 `&` 规则** → 数组名（`name`）不用 &，普通变量（`&year`）要 &。
6. **`%s` 读不了带空格** → 用 `%[^\n]` 读到换行。
7. **snprintf 返回值 = "若空间足够会写的字符数"**，可能大于实际写入 —— 判截断用它与 `size` 比较。
8. **行超长**：`fgets` 只读 `n-1` 个字符，剩余留在流里 —— 需循环读完或加大缓冲区。

## 6. 一句话总结

**fgets 安全读一行（保留换行、给 n）；sscanf 从字符串按格式拆字段（判返回值、数组名不用 &）；sprintf 把数据按格式拼成字符串（用 snprintf 防溢出）。三者常配合：fgets 读行 → sscanf 解析 → sprintf 拼装。**
