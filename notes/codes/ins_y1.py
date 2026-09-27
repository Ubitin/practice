# -*- coding: utf-8 -*-
# 手册新增 §9.3：变量名撞 math.h 的贝塞尔函数（y0/y1/yn/j0/j1/jn）
import io, os, glob

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = [f for f in glob.glob(os.path.join(d, "*.md")) if f.endswith("机试代码模板速查手册.md")][0]
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

i = next(i for i, l in enumerate(lines) if l.startswith("## 10. lambda"))
print("anchor L%d: %s" % (i + 1, lines[i]))

blk = [
 '### 9.3 报错 `redeclared as different kind of entity`：变量名撞上了 math.h 的函数',
 '```text',
 '【症状】评测机报（本地可能编得过）：',
 '    error: \'ll y1\' redeclared as different kind of entity',
 '      note: previous declaration \'double y1(double)\'        ← math.h 里的贝塞尔函数！',
 '    error: invalid operands of types \'ll\' and \'double(double)\' to binary \'operator*\'',
 '    warning: the address of \'double y1(double)\' will never be NULL [-Waddress]',
 '',
 '【实测：换标准/模式就复现，与代码逻辑无关】',
 '    同一份代码 grep -std：',
 '      -std=c++17 / c++20 / c++23 : 退出码 0、零警告   ✅（严格模式里贝塞尔函数不可见）',
 '      -std=gnu++17 / gnu++20      : 退出码 1、12 行报错 ✘（GNU 扩展暴露了 POSIX 数学函数）',
 '    ⇒ 很多 OJ 与 IDE 默认用 gnu++ 模式，所以"本地能编、交上去报错"',
 '',
 '【罪魁名单：<math.h>/<cmath> 里那些"看起来像变量名"的函数】',
 '    y0  y1  yn        第二类贝塞尔函数',
 '    j0  j1  jn        第一类贝塞尔函数',
 '    （都带 double 参数、返回 double；在 gnu 模式下位于全局命名空间）',
 '    ⚠️ 还有 j0/j1 在 <complex> 里也有同名重载，撞上更难查',
 '',
 '【对策】',
 '    1. 变量改名：y1 → dy1 / den1 / mom1 / yy1（**最省事，推荐**）',
 '       ✘ ll x1, y1, x2, y2;    ✔ ll x1, dy1, x2, dy2;',
 '    2. 把变量放进 main 内部（局部作用域，但全局函数仍可能在查找时被选中 ⇒ 不如改名稳）',
 '    3. 不用 <bits/stdc++.h>，只包含需要的头（能减少撞名概率，但 std::sqrt 等仍要 cmath）',
 '    4. 本地自测加一条：g++ -std=gnu++20 -O2 -Wall a.cpp  ← 提前暴露这类名字冲突',
 '```',
 '// 一句话判据：**报错说"redeclared as different kind of entity"，就去搜你的标识符是不是和标准库/系统头里的函数重名**',
 '// 同族：count（<algorithm> 有 std::count）、distance、left、right、time、index、remove、abs、pow、div、exception',
 '//      —— 变量名尽量带后缀：cnt_、dis_、lf_、rt_、tim_、idx_',
 ''
]
for k, s in enumerate(blk):
    lines.insert(i + k, s)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))

# 体检
fences = [j for j, l in enumerate(lines) if l.lstrip().startswith("```")]
inside = False
bad = 0
for l in lines:
    if l.lstrip().startswith("```"):
        inside = not inside
        continue
    if inside and (l.startswith("## ") or l.startswith("### ")):
        bad += 1
print("行数 =", len(lines), " 围栏 =", len(fences), "偶数 =", len(fences) % 2 == 0, " 块内标题 =", bad)
