# -*- coding: utf-8 -*-
# 索引登记：新增《辗转相除求gcd》讲义行 + 检索表 2 行 + 日志 + 收录数
import io, os, glob, re

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = os.path.join(d, "00-知识索引与复习路线图.md")
expect = len(glob.glob(os.path.join(d, "*.md"))) - 2

lines = io.open(p, "r", encoding="utf-8").read().split("\n")
lines[4] = re.sub(r"收录：\*\*\d+ 份\*\*", "收录：**%d 份**" % expect, lines[4])

# ① 线 6 表尾（锚定"组合数计算"那行之后）
i0 = next(i for i, l in enumerate(lines) if l.startswith("| `组合数计算-有没有现成函数`"))
row = (
 "| `辗转相除求gcd-那行while在干什么` | **回答「`01_fraction_fixed.cpp` 第 9 行在干什么」**：⭐ **那就是欧几里得算法（辗转相除法）的核心**"
 "`while (b) { ll t = a % b; a = b; b = t; }` —— 反复把 `(a,b)` 换成 `(b, a%b)`，`b==0` 时 `a` 就是 gcd；"
 "**手推 `gcd(12,8)`：(12,8)→(8,4)→(4,0) ⇒ 4**；为什么对：`d | a 且 d | b ⇒ d | (a%b)`，公约数集合不变而数字递减 ⇒ 必收敛。"
 "**`t` 必须先存**（下一句 `a=b` 会覆盖 `a`）。第 7、8 行 `if (a<0) a=-a;` 是因为 **C++ 的 `%` 保留符号**（`-12%8 == -4`），"
 "不取绝对值就会得到负 gcd ⇒ 分数符号搞反。**⚠️ 并更正我自己上一条讲重的地方**：我说过「从 `min(x,y)` 起手的试除法遇负数会一次都不执行」"
 "—— **实测证明这句是错的**：`gcd_trial(-12,8)=4`、`(12,-8)=4`、`(-12,-8)=4` 全对（因为你的代码本来就有 `if(a<0)a=-a;`）。"
 "**试除法真正的问题是「慢」**：实测 `gcd(99999999,100000000)`（互素最坏情况）**试除法 132.0 ms vs 辗转相除 <0.001 ms**，"
 "外推到 1e18 规模约 **1.3e12 ms ≈ 42 年** ⇒ 必 TLE。**连带两件事**：① **`lcm` 必须先除后乘**（`a/g*b`，写成 `a*b/g` 会溢出）；"
 "② **C++17 有 `std::gcd` / `std::lcm`**（`<numeric>`），实测 `std::gcd(-12,8)=4`（自带处理符号）、"
 "`std::gcd(999999999999999989,1000000000000000000)=1`，**手写那 6 行可以简化成一行**。另收分数化简标准三步、`gcd(x,0)=|x|`、"
 "`gcd(0,0)=0`、区间 gcd 的三种做法（前缀不可用/ST 表/线段树）与 6 题自查。手册新增 **§6.17 gcd / lcm** |"
)
lines.insert(i0 + 1, row)

# ② 检索表：插到「组合数」相关行之后（找含 §6.16 的行）
i1 = next(i for i, l in enumerate(lines) if "§6.16" in l and l.lstrip().startswith("|"))
rows = [
 '| 最大公约数怎么写 / 为什么这么写 | 辗转相除求gcd-那行while在干什么 §一 | **辗转相除** `while(b){t=a%b;a=b;b=t;}` 返回 `a`；先取绝对值（`%` 保留符号）。实测：互素最坏情况**试除法 132 ms vs 辗转相除 <0.001 ms** |',
 '| 最小公倍数怎么算不溢出 | 辗转相除求gcd-那行while在干什么 §4.1 | ⭐ **必须先除后乘**：`a / gcd(a,b) * b`；写成 `a*b/g` 时 `a*b` 会先溢出。或直接用 C++17 的 `std::lcm` |',
]
for k, r in enumerate(rows):
    lines.insert(i1 + 1 + k, r)

# ③ 日志
log = (
 "| 2026-09-27 | +`辗转相除求gcd-那行while在干什么`（**线 6 · 综合**，回答「`01_fraction_fixed.cpp` 第 9 行在干什么」；"
 "**本轮同时补做前面遗漏的交付物**：这轮此前只回答了问题、没做讲义/索引/手册，违反 §3.3 交付物四条，现补齐）："
 "第 9 行 = **欧几里得算法（辗转相除法）**，`(a,b)→(b,a%b)` 直到 `b==0`；手推 `gcd(12,8)=4`；`t` 必须先存；"
 "第 7/8 行取绝对值是因为 `%` 保留符号（`-12%8==-4`）。**⚠️ 并更正我自己的错误结论**：此前称「试除法遇负数会不执行」——"
 "实测 `gcd_trial(-12,8)=4`、`(12,-8)=4`、`(-12,-8)=4` **全对**，因为你的代码本就有 `if(a<0)a=-a;`；"
 "**试除法真正的问题是慢**：`gcd(99999999,100000000)` **132.0 ms vs 辗转相除 <0.001 ms**（外推 1e18 规模 ≈ 42 年）。"
 "另收 `lcm` 必须**先除后乘**、C++17 `std::gcd`/`std::lcm` 一行替代、分数化简三步、区间 gcd 三法、6 题自查。"
 "**手册新增 §6.17 gcd / lcm（辗转相除、溢出安全的 lcm、std::gcd）**；索引线 6 + 检索表 +2 行 → 收录数 **113 → " + str(expect) + " 份** |"
)
last = max(i for i, l in enumerate(lines) if l.startswith("| 2026-"))
lines.insert(last + 1, log)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
print("done, total lines =", len(lines), " 收录 =", expect)
