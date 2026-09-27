# 变量名 `y1` 撞上 math.h 的贝塞尔函数 · 一次"本地能编、线上报错"的编译错误

> **触发点**：你贴出报错 `error: invalid operands of types 'll' and 'double(double)' to binary 'operator*'`，位置在 `01.cpp:32`，问"为什么我的代码还是报错"。
> **一句话结论**：**你的全局变量名 `y1` 和 `<math.h>` 里的贝塞尔函数 `double y1(double)` 撞名了**。
> 关键证据：报错里那个神秘类型 **`double(double)`** —— 它不是你的类型，而是**函数类型**；编译器把 `y1` 当成了那个数学函数，于是 `y1 * y2` 变成"函数 × 整数"。
> **修法：把 `y1` 改名**（如 `dy1`）即可，一分钟的事。

---

## 一、报错原文与逐句解读

```
01.cpp:4:21: error: 'll y1' redeclared as different kind of entity
    4 | string s1,s2; ll x1,y1,x2,y2;
      |                     ^~
  note: previous declaration 'double y1(double)'        ← ★ 元凶在这里
D:/.../include/math.h:312:24: note: ... double __cdecl y1 (double) __MINGW_ATTRIB_DEPRECATED_MSVC2005;

01.cpp:32:75: error: invalid operands of types 'll' {aka 'long long int'} and 'double(double)' to binary 'operator*'
      |                                                   ~~ ^ ~~
      |                                                   |    |
      |                                                   |    double(double)   ← ★ 这是一个【函数类型】
      |                                                   ll {aka long long int}
```

| 报错片段 | 意思 |
|---|---|
| `redeclared as different kind of entity` | `y1` 这个名字已经被声明过了，而且**类型不同**（你的是 `ll`，系统头里是函数） |
| `previous declaration 'double y1(double)'` | 之前的声明是"**接收 double、返回 double 的函数**" |
| `types 'll' and 'double(double)'` | 所以 `y1 * y2` 被理解成 "`long long` × **函数指针**" ⇒ 非法 |
| `the address of 'double y1(double)' will never be NULL [-Waddress]` | 编译器把你的 `y1`（当函数用）拿去判真假，于是抱怨"函数地址不可能为空" |

---

## 二、为什么"本地不报、评测机报"？——是 `-std` 模式差异（实测）

`<math.h>` 里的贝塞尔函数族属于 **POSIX/GNU 扩展**，在**严格标准模式**下不会暴露到全局命名空间：

| 编译模式 | 结果（我实测同一份 `01.cpp`） |
|---|---|
| `-std=c++17` | ✅ 退出码 0、零警告 |
| `-std=c++20` | ✅ 退出码 0、零警告 |
| `-std=c++23` | ✅ 退出码 0、零警告 |
| **`-std=gnu++17`** | ❌ 退出码 1、**12 行报错** |
| **`-std=gnu++20`** | ❌ 退出码 1、**12 行报错** ← 和你的截图**一字不差** |

⭐ **很多 OJ 和 IDE（CodeBlocks / Dev-C++）默认就是 `gnu++` 模式**，所以出现"本地能编、交上去报错"。
⇒ 你截图里的报错和 `-std=gnu++20` 下完全一致 ⇒ **可以确定评测环境是 GNU 扩展模式**。

---

## 三、罪魁名单：`<math.h>` 里那些"看起来像变量名"的函数

| 名字 | 真实身份 |
|---|---|
| **`y0`、`y1`、`yn`** | **第二类**贝塞尔函数 |
| `j0`、`j1`、`jn` | **第一类**贝塞尔函数 |
| ⚠️ 补充 | `j0/j1` 在 `<complex>` 里还有同名重载，撞上更难查 |

它们的共同特征：**接收 `double`、返回 `double`、名字短得像变量**。所以 `x1, y1, x2, y2` 这种命名里，**只有 `y1` 会炸**（`x1`、`x2`、`y2` 都不是数学函数名）——这也解释了为什么你只在这一个变量上报错。

---

## 四、修法（三种，推荐第一种）

### ✔ 方案 1（最省事）：改名

```cpp
string s1,s2;
ll x1, y1, x2, y2;        // ✘ y1 撞 math.h 的贝塞尔函数
ll x1, dy1, x2, dy2;      // ✔ 加个前缀就躲开了
// 其它可选名：den1 / mom1 / yy1 / frac1b
```

### 方案 2：放进局部作用域

变量写进 `main()` 里能降低撞名概率，但**全局函数在名字查找时仍可能被优先选中**（尤其配合 `using namespace std;`），所以**不如改名稳**。

### 方案 3：别用 `<bits/stdc++.h>`

只包含需要的头能减少撞名面，但代价是失去"万能头"的便利，而且 `std::sqrt` 之类还是要 `<cmath>` ⇒ **不划算**。

### 顺带一个自测习惯

本地编译时**多加一条 gnu 模式**，就能提前暴露这类名字冲突：

```
g++ -O2 -std=gnu++20 -Wall -Wextra 01.cpp -o a      ← 提前抓
```

---

## 五、实测验证

我把 `y1 → dy1` 改名后的版本（`test_\01_fraction_fixed3.cpp`）在**7 种标准/模式**下全跑了一遍：

| 标准 | 退出码 | 错误/警告 |
|---|---|---|
| `c++11` | 0 | 0 ✅ |
| `c++14` | 0 | 0 ✅ |
| `c++17` | 0 | 0 ✅ |
| `c++20` | 0 | 0 ✅ |
| `gnu++17` | 0 | 0 ✅ ← 原版在这里炸 |
| `gnu++20` | 0 | 0 ✅ ← 原版在这里炸 |
| `c++23` | 0 | 0 ✅ |

运行结果也正确：`1/2 1/3` → `(1/2)+(1/3)=5/6`、`1/6`、`1/6`、`3/2` ✅

⚠️ **并如实记录我自己的两次判断失误**：
1. 我先猜是"评测机用 C++11/14，没有 `std::gcd`"——**你告知用的是 C++20，这个诊断不成立**，我随即撤回（那次手册插入脚本也没执行成功，无需回滚）；
2. 真正的原因（`y1` 撞贝塞尔函数）是**你贴的截图里的 `double(double)` 这个类型**点醒的 —— 我最初只盯着自己本地的 `-std=c++17` 编译结果说"编译通过"，**没有换 gnu 模式试**。
⇒ 教训：**评审编译错误时，必须把 `-std` 的几种模式都跑一遍**，不能只用一个标准下结论。

---

## 六、连带发现：你这版还有 2 个会导致 WA 的格式问题

| 输入 | 你的输出 | 应为 |
|---|---|---|
| `0/5 1/2` | `(0/5)/(1/2)=`**`0`** ✘ | `0/1`（第 10 行 `return "0"` 少了分母） |
| 分母为负 | 原样输出 `1/-2` | 符号提到分子 `-1/2` |

（修法见 `test_\01_fraction_fixed3.cpp`：分子为 0 返回 `"0/1"`，并加 `if (y < 0) { x = -x; y = -y; }`。）

---

## 七、自查

<details><summary>Q1：报错里的 `double(double)` 是什么？为什么它能帮你定位问题？</summary>

它是 C++ 表示"**函数类型**"的写法：`double(double)` = 接收一个 `double`、返回 `double` 的函数。
因为 `y1` 被解析成函数，所以 `y1` 的类型就是 `double(double)`（作为表达式时是函数指针 `double(*)(double)`）。
⭐ **看到报错里出现"看起来像函数"的类型，第一反应就该是"我的标识符和某个函数重名了"**——这是最快的一条线索。

</details>

<details><summary>Q2：为什么只有 `y1` 报错，`x1`、`x2`、`y2` 都没事？</summary>

因为 `<math.h>` 里只有 `y0/y1/yn`、`j0/j1/jn` 这几个名字，**没有 `x1`、`x2`、`y2`**。
所以 `x1, y1, x2, y2` 里恰好只有 `y1` 撞上。这也说明：**"为什么偏偏是这个变量"往往能在头文件里找到答案**。

</details>

<details><summary>Q3：为什么严格 `-std=c++17` 就不报错？</summary>

贝塞尔函数 `y0/y1/yn/j0/j1/jn` 属于 **POSIX/XSI 扩展**（不是标准 C++），GCC 只在 `gnu++` 模式（`-std=gnu++XX`）或定义了相应特性宏时把它们放进全局命名空间。
`-std=c++XX` 是严格模式，这些扩展被隐藏 ⇒ 你的 `y1` 变量就不冲突了。
⚠️ 但**别因此就去用严格模式"绕过"**——评测机用什么模式你控制不了，**改名才是正解**。

</details>

<details><summary>Q4：还有哪些"短名字"容易撞？</summary>

按危险程度排：
1. **系统/数学**：`y0 y1 yn j0 j1 jn`、`index`（POSIX 有 `index()`）、`time`、`div`、`abs`、`pow`、`left`/`right`（`<ios>` 的操纵符）
2. **STL**：`count`、`distance`、`remove`、`swap`、`data`、`size`、`begin`/`end`
3. **Windows 头**：`min`/`max`（老 `<windows.h>` 里是宏，被 `#define` 掉会炸得更惨）
⇒ **对策**：竞赛里给变量加后缀/前缀：`cnt_`、`dis_`、`lf_`、`rt_`、`tim_`、`idx_`；或者干脆避开这两个字母组合。

</details>

<details><summary>Q5：改完名还是报错怎么办？</summary>

按顺序做三件事：
1. **看第一个 error 的行号**（后面的 error 常常是第一个的连锁反应）；
2. **换 gnu 模式再编一次**：`g++ -std=gnu++20 -Wall -Wextra a.cpp`；
3. **把报错里出现的"陌生类型"抄出来搜**（比如 `double(double)`、`__int128`），它往往直接指向冲突来源。
</details>
