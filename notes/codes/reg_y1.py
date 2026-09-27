# -*- coding: utf-8 -*-
# 索引登记：y1 撞贝塞尔函数 讲义行 + 检索表 2 行 + 日志 + 收录数
import io, os, glob, re

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = os.path.join(d, "00-知识索引与复习路线图.md")
expect = len(glob.glob(os.path.join(d, "*.md"))) - 2

lines = io.open(p, "r", encoding="utf-8").read().split("\n")
lines[4] = re.sub(r"收录：\*\*\d+ 份\*\*", "收录：**" + str(expect) + " 份**", lines[4])

# ① 线 6 表尾（锚点：冲赛14天计划那行属于线 6）
i6 = next(i for i, l in enumerate(lines) if l.startswith("| `冲赛14天计划-hello算法读不读完`"))
# 找线 6 区间内最后一个以 "| `" 开头的行
h6 = next(i for i, l in enumerate(lines) if l.startswith("### 线 6"))
h7 = next(i for i, l in enumerate(lines) if l.startswith("### 线 7"))
last6 = max(i for i in range(h6, h7) if lines[i].startswith("| `"))
row = (
 "| `变量名y1撞math.h贝塞尔函数-本地能编线上报错` | **回答「为什么我的代码还是报错」**（用户贴 `-std=gnu++20` 的报错截图）：⭐ **根因是全局变量名 `y1` 撞上 `<math.h>` 的贝塞尔函数 `double y1(double)`**；"
 "**关键线索 = 报错里的陌生类型 `double(double)`（那是函数类型）**，于是 `y1 * y2` 变成「`long long` × 函数指针」⇒ `invalid operands of types 'll' and 'double(double)'`；"
 "另有 `error: 'll y1' redeclared as different kind of entity` + `previous declaration 'double y1(double)'`。"
 "**为什么「本地能编、线上报错」**（实测同一份代码换模式）：`-std=c++17 / c++20 / c++23` **全部退出码 0、零警告**；"
 "**`-std=gnu++17` / `gnu++20` 退出码 1、12 行报错**（GNU 扩展把 POSIX 数学函数暴露到全局命名空间）——"
 "**很多 OJ 与 CodeBlocks/Dev-C++ 默认就是 `gnu++` 模式**，且 gnu++20 的报错与用户截图**一字不差** ⇒ 可判定评测环境是 GNU 扩展模式。"
 "**罪魁名单**：`y0 y1 yn`（第二类贝塞尔）、`j0 j1 jn`（第一类贝塞尔），<complex> 里还有同名重载；"
 "`x1 / x2 / y2` 都没事，**这解释了「为什么偏偏是 y1」**。**修法**：改名 `y1 → dy1`（最省事）；或别用 `bits/stdc++.h`；"
 "并在本地自测时**多加一条 `-std=gnu++20`** 提前暴露。**实测**：改名后的 `test_\\01_fraction_fixed3.cpp` 在 "
 "`c++11/14/17/20/23 + gnu++17/gnu++20` **7 种模式全部 0 错误 0 警告**，运行输出正确（`1/2 1/3` → `5/6, 1/6, 1/6, 3/2`）。"
 "**连带修掉 2 个 WA 格式问题**：分子为 0 时 `simple()` 返回裸 `0` ⇒ 改 `0/1`；分母为负时没翻符号 ⇒ 加 `if (y<0){x=-x;y=-y;}`。"
 "⚠️ **并如实记录我的两次判断失误**：① 先猜「评测机是 C++11/14、没有 `std::gcd`」——用户告知用 C++20，**诊断不成立、已撤回**；"
 "② 最初只在自己本地的 `-std=c++17` 下编译就说「编译通过」，**没换 gnu 模式试**，是用户截图里的 `double(double)` 点醒了我。"
 "⇒ 提炼「**评审编译错误必须把 `-std` 的几种模式都跑一遍**」。另收「短名字易撞清单」（`y0/y1/yn/j0/j1/jn`、`index/time/div/abs`、`count/distance/remove`、Windows 的 `min/max` 宏）与 5 题自查。**手册新增 §9.3** |"
)
lines.insert(last6 + 1, row)

# ② 检索表 2 行（插到「最大公约数怎么写」那行之后）
i_lookup = next(i for i, l in enumerate(lines) if l.startswith("| 最大公约数怎么写"))
rows = [
 '| `redeclared as different kind of entity` 怎么修 | 变量名y1撞math.h贝塞尔函数-本地能编线上报错 §一 | ⭐ **变量名和系统头里的函数重名**：`ll y1` 撞上 `<math.h>` 的贝塞尔函数 `double y1(double)`。线索 = 报错里出现**函数类型** `double(double)`。**改名即可**（`y1 → dy1`） |',
 '| 本地能编、评测机报编译错误 | 变量名y1撞math.h贝塞尔函数-本地能编线上报错 §二 | 多为 **`-std` 模式差异**：实测同一份代码 `-std=c++17/c++20/c++23` 全过，**`gnu++17/gnu++20` 报 12 行错**（GNU 扩展暴露 POSIX 数学函数）。⇒ 本地自测**多加一条 `-std=gnu++20`** |',
]
for k, r in enumerate(rows):
    lines.insert(i_lookup + 1 + k, r)

# ③ 日志
log = (
 "| 2026-09-27 | +`变量名y1撞math.h贝塞尔函数-本地能编线上报错`（**线 6 · 综合**，回答「为什么我的代码还是报错」）："
 "⭐ **根因 = 全局变量 `y1` 撞 `<math.h>` 的贝塞尔函数 `double y1(double)`**；关键线索是报错里的函数类型 **`double(double)`**。"
 "**实测模式差异**：同一份 `01.cpp` 在 `-std=c++17/c++20/c++23` 下**全过**，在 **`gnu++17/gnu++20` 下报 12 行错**"
 "（gnu++20 的报错与用户截图一字不差 ⇒ 评测环境是 GNU 扩展模式）。罪魁名单 `y0/y1/yn/j0/j1/jn`；"
 "改名后 `01_fraction_fixed3.cpp` 在 **7 种标准/模式全部 0 错误 0 警告**。"
 "**⚠️ 如实记录我两次判断失误**：① 先猜「评测机是 C++11/14 缺 `std::gcd`」，用户指出用 C++20 ⇒ **诊断作废、已撤回**；"
 "② 最初只按本地 `-std=c++17` 的编译结果说「编译通过」，**没换 gnu 模式**，是用户截图点醒。"
 "**手册新增 §9.3 报错 `redeclared as different kind of entity`（变量名撞 math.h 函数）**并重跑 docx；"
 "索引线 6 + 检索表 +2 行 → 收录数 **114 → " + str(expect) + " 份** |"
)
last = max(i for i, l in enumerate(lines) if l.startswith("| 2026-"))
lines.insert(last + 1, log)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
print("done, lines =", len(lines), " 收录 =", expect)
