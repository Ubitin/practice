# -*- coding: utf-8 -*-
# 索引日志追加：substr 返回值被丢弃的实测排错
import io, os

p = r"D:\lenovo\chat_with_deepseek_harness\讲义\00-知识索引与复习路线图.md"
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

log = (
 "| 2026-09-27 | **排错实录（用户新写的 `01.cpp`：分数四则运算）**——「我的代码为什么报错了」："
 "**代码其实编译通过**（退出码 0），真正的错是【逻辑静默失效】：`son1.substr(0,pos1); mom1.substr(pos1+1);` 这两句"
 "**没有接住返回值** ⇒ `substr` 不改原串、只返回新串 ⇒ `son1`/`mom1` 仍是 `\"1/2\"` ⇒ `stoi(\"1/2\")` **遇 `/` 就停**、返回 1 "
 "⇒ **两个分数的分母都变成 1**，`1/2` 与 `1/3` 全被当成 `1/1`（实测输出 `(1/1)+(1/1)=2/1`，本应 `(1/2)+(1/3)=5/6`）。"
 "**编译器早就警告**：`warning: ignoring return value of '...substr...' [-Wunused-result]`（L61/L62 各两条）。"
 "**同题另暴露 4 个坑**：① `if(!x % i)` 想表达整除、实际是 `(!x) % i`（必须 `x % i == 0`）；"
 "② 化简循环从 `min(x,y)` 起手 ⇒ **负数时循环不执行、永不化简**（应改用标准 gcd + 取绝对值）；"
 "③ 分子为 0 要特判输出 `0/1`；④ 除法可能令分母为 0（除数的分子为 0）。"
 "**修正版 `test_\\01_fraction_fixed.cpp` 实测**：样例 `1/2 1/3` → `5/6, 1/6, 1/6, 3/2` ✅；"
 "**vs Python `fractions.Fraction` 对拍 49 组（含负数/0/大数）不一致 0 ✅**；`-Wall -Wextra` 0 警告。"
 "手册新增 **§2.2.1「substr 的返回值必须接住」**（含实测证据、同族陷阱清单、另外 4 个坑）并重跑 docx"
 "（141044 字节、1659 行代码、超宽 0、围栏 156 偶数全闭合、块内标题 0）|"
)
last = max(i for i, l in enumerate(lines) if l.startswith("| 2026-"))
lines.insert(last + 1, log)
io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
print("日志已追加，总行数 =", len(lines))
